/**
 * @file src/verifier/verifier.cpp
 * @author RenoirTan
 * @brief Implementation for the Fidgety::Verifier class along with its helper
 * classes/functions.
 * @version 0.1
 * @date 2022-03-14
 * 
 * @copyright Copyright (c) 2022
 */

#include <random>
#include <set>
#include <spdlog/spdlog.h>
//#include <fidgety/extensions.hpp>
#include <fidgety/verifier.hpp>
#include <fidgety/_utils.hpp>

using namespace Fidgety;

template <typename T>
static inline void _unionSet(std::set<T> &a, const std::set<T> &b) {
    for (const T& thing : b) {
        a.emplace(thing);
    }
}

static inline bool _optionHasOption(const Option &option, const OptionIdentifier &identifier) {
    // assuming that option is nested list
    for (const auto &child : option.getNestedList()) {
        if (option.getIdentifier() + child == identifier) {
            return true;
        }
    }
    return false;
}

static inline bool _isOrphan(
    const VerifierManagedOptionList &vmol,
    const OptionIdentifier &identifier,
    size_t currentDepth,
    const std::set<OptionIdentifier> notOrphans
) {
    spdlog::trace("[_isOrphan] currentDepth: {}", currentDepth);
    for (const auto &notOrphanId : notOrphans) {
        spdlog::trace("[_isOrphan] comparing '{}'", notOrphanId);
        size_t notOrphanIdDepth = notOrphanId.depth();
        spdlog::trace("[_isOrphan] notOrphanIdDepth: {}", notOrphanIdDepth);
        if (notOrphanIdDepth != currentDepth-1) {
            continue;
        }
        auto notOrphan = vmol.find(notOrphanId);
        if (notOrphan == vmol.end()) {
            continue;
        }
        if (
            notOrphan->second->getValueType() == OptionValueType::NESTED_LIST &&
            _optionHasOption(*(notOrphan->second), identifier)
        ) {
            spdlog::trace("[_isOrphan] '{}' has '{}'", notOrphanId, identifier);
            return false;
        } else {
            spdlog::trace("[_isOrphan] '{}' does not have '{}'", notOrphanId, identifier);
        }
    }
    return true;
}

static inline std::vector<OptionIdentifier> _findOrphansInOption(
    const VerifierManagedOptionList &vmol,
    const OptionIdentifier &identifier
) {
    auto myIt = vmol.find(identifier);
    const auto &option = myIt->second;
    if (myIt == vmol.end() || option->getValueType() != OptionValueType::NESTED_LIST) {
        return {};
    }
    std::vector<OptionIdentifier> orphans;
    for (const auto &childName : option->getNestedList()) {
        OptionIdentifier child = identifier + childName;
        if (vmol.find(child) == vmol.end()) {
            orphans.push_back(std::move(child));
        }
    }
    return orphans;
}

static VerifierStatus _findOrphans(
    const VerifierManagedOptionList &vmol,
    std::set<OptionIdentifier> &orphans
) {
    const size_t nOptions = vmol.size();
    size_t nAccounted = 0;
    std::set<OptionIdentifier> notOrphans;
    for (const auto &idOpPair : vmol) {
        const OptionIdentifier &identifier = idOpPair.first;
        if (identifier.depth() == 1) {
            notOrphans.emplace(identifier);
        }
    }
    nAccounted += notOrphans.size();
    size_t currentDepth = 2;
    while (nAccounted < nOptions) {
        std::set<OptionIdentifier> currentUnorphanedBuffer;
        for (const auto &idOpPair : vmol) {
            const OptionIdentifier &identifier = idOpPair.first;
            if (identifier.depth() == currentDepth) {
                spdlog::trace("[_findOrphans] checking '{}'", identifier);
                if (_isOrphan(vmol, identifier, currentDepth, notOrphans)) {
                    spdlog::debug("[_findOrphans] _isOrphan: {}", identifier);
                    orphans.emplace(identifier);
                } else {
                    spdlog::debug("[_findOrphans] !_isOrphan: {}", identifier);
                    currentUnorphanedBuffer.insert(identifier);
                }
                ++nAccounted;
            }
        }
        _unionSet(notOrphans, currentUnorphanedBuffer);
        ++currentDepth;
    }
    return VerifierStatus::Ok;
}

static VerifierStatus _purgeOrphans(
    VerifierManagedOptionList &vmol,
    const std::set<OptionIdentifier> &orphans
) {
    for (const auto &identifier : orphans) {
        auto it = vmol.find(identifier);
        if (it != vmol.end()) {
            vmol.erase(it);
        }
    }
    return VerifierStatus::Ok;
}

std::string VerifierException::codeAsErrorType(void) const {
    switch (mCode) {
        case 0: return "Ok";
        case 1: return "FileNotFound";
        case 2: return "CannotReadFile";
        case 3: return "CannotWriteFile";
        case 4: return "CannotCloseFile";
        case 5: return "ResourceBusy";
        case 6: return "CannotOpenMultipleFiles";
        case 7: return "FilesNotOpen";
        case 8: return "SyntaxError";
        case 9: return "WeakPointerExpired";
        case 10: return "OptionDoesNotExist";
        case 11: return "Implemented";
        default: return "Other";
    }
}

const char *VerifierException::getSimpleWhat(void) const noexcept {
    return "A Fidgety::VerifierException occurred.";
}

VerifierIdentifier createIdentifier(void) {
    spdlog::trace("Creating identifier in fidgety/src/verifier/verifier.cpp");
    VerifierIdentifier identifier;
    std::uniform_int_distribution<uint32_t> random;
    std::default_random_engine engine;
    for (auto &idBlock : identifier) {
        idBlock = random(engine);
    }
    return identifier;
}

class Fidgety::VerifierInner {
    public:
        VerifierInner(std::unique_ptr<ValidatorContextCreator> &&contextCreator) :
            mContextCreator(std::move(contextCreator)),
            mIdentifier(createIdentifier())
        {
            spdlog::trace("Created Fidgety::VerifierInner with contextCreator.");
        }

        VerifierInner(
            VerifierManagedOptionList &&options,
            std::unique_ptr<ValidatorContextCreator> &&contextCreator
        ) :
            mContextCreator(std::move(contextCreator)),
            mIdentifier(createIdentifier()),
            mOptions(options)
        {
            spdlog::trace("Created Fidgety::VerifierInner with options, contextCreator.");
        }

        ~VerifierInner(void) {
            spdlog::trace("Deleting Fidgety::VerifierInner.");
        }

        VerifierInner(VerifierInner &&inner) = delete;
        VerifierInner(const VerifierInner &inner) = delete;
        VerifierInner &operator=(VerifierInner &&inner) = delete;
        VerifierInner &operator=(const VerifierInner &inner) = delete;

        size_t numberOfLocks(void) const {
            return mLocks.size();
        }

        bool optionExists(const OptionIdentifier &identifier) const {
            spdlog::trace("Checking if option exists in Fidgety::VerifierInner.");
            return mOptions.find(identifier) != mOptions.end();
        }

        bool isOptionLocked(const OptionIdentifier &identifier) const {
            spdlog::trace("Checking if option is locked in Fidgety::VerifierInner.");
            return mLocks.find(identifier) != mLocks.end();
        }

        std::weak_ptr<Option> lockOption(const OptionIdentifier &identifier) {
            spdlog::debug("Locking option '{0}' in Fidgety::VerifierInner.", identifier);
            auto option = mOptions.find(identifier);
            if (option == mOptions.end()) {
                FIDGETY_CRITICAL(
                    VerifierException,
                    VerifierStatus::OptionDoesNotExist,
                    "Cannot find Fidgety::Option with name: {0}.",
                    identifier
                );
            }
            if (isOptionLocked(identifier)) {
                FIDGETY_CRITICAL(
                    VerifierException,
                    VerifierStatus::ResourceBusy,
                    "Fidgety::Option '{0}' is currently being used.",
                    identifier
                );
            }
            mLocks.insert(identifier);
            spdlog::debug(
                "New lock created for option '{0}' in Fidgety::VerifierInner",
                identifier
            );
            return std::weak_ptr<Option>(option->second);
        }

        void releaseLockOnDrop(VerifierOptionLock &&lock) {
            spdlog::debug("Silentyl releasing lock in Fidgety::VerifierInner.");
            if (lock.optionExists()) {
                const Option &option = lock.getOption();
                const OptionIdentifier &identifier = option.getIdentifier();
                auto set_lock_it = mLocks.find(identifier);
                if (set_lock_it == mLocks.end()) {
                    spdlog::warn(
                        "Lock for '{0}' was not found in Fidgety::VerifierInner::mLocks.",
                        identifier
                    );
                } else {
                    mLocks.erase(set_lock_it);
                }
            }
        }

        ValidatorMessage releaseLock(VerifierOptionLock &&lock) {
            spdlog::debug("Releasing lock in Fidgety::VerifierInner.");
            if (!lock.optionExists()) {
                FIDGETY_CRITICAL(
                    VerifierException,
                    VerifierStatus::OptionDoesNotExist,
                    "Could not find option when releasing lock from Fidgety::VerifierInner."
                );
            }
            Option &option = lock.getMutOption();
            const OptionIdentifier &identifier = option.getIdentifier();
            spdlog::trace(
                "Looking for registered lock for '{0}' in Fidgety::VerifierInner",
                identifier
            );
            auto set_lock_it = mLocks.find(identifier);
            ValidatorContext context = mContextCreator->createContext(mOptions, identifier);
            ValidatorMessage message = option.validate(context);
            if (set_lock_it == mLocks.end()) {
                spdlog::warn(
                    "Lock for '{0}' was not found in Fidgety::VerifierInner::mLocks.",
                    identifier
                );
            } else {
                mLocks.erase(set_lock_it);
            }
            spdlog::debug("Lock released in Fidgety::VerifierInner");
            return message;
        }

        const ValidatorContextCreator &getContextCreator(void) const {
            return *mContextCreator;
        }

        ValidatorContextCreator &getMutContextCreator(void) {
            return *mContextCreator;
        }

        const VerifierIdentifier &getIdentifier(void) const {
            return mIdentifier;
        }

        VerifierIdentifier &getMutIdentifier(void) {
            return mIdentifier;
        }

        const VerifierManagedOptionList &getOptionList(void) const {
            return mOptions;
        }

        VerifierManagedOptionList &getMutOptionList(void) {
            return mOptions;
        }

        const std::set<OptionIdentifier> &getLocks(void) const {
            return mLocks;
        }

        std::set<OptionIdentifier> &getMutLocks(void) {
            return mLocks;
        }

        VerifierStatus findOrphanedOptions(std::set<OptionIdentifier> &orphans) {
            return _findOrphans(mOptions, orphans);
        }

        VerifierStatus purgeOrphanedOptions(const std::set<OptionIdentifier> &orphans) {
            return _purgeOrphans(mOptions, orphans);
        }

    protected:
        std::unique_ptr<ValidatorContextCreator> mContextCreator;
        VerifierIdentifier mIdentifier;
        VerifierManagedOptionList mOptions;
        std::set<OptionIdentifier> mLocks;
};

ValidatorContext ValidatorContextCreator::createContext(
    const VerifierManagedOptionList &verifier,
    const OptionIdentifier &identifier
) {
    spdlog::trace(
        "Creating Fidgety::ValidatorContext in Fidgety::ValidatorContextCreator for '{0}'",
        identifier
    );
    return ValidatorContext();
}

VerifierOptionLock::VerifierOptionLock(
    const std::weak_ptr<VerifierInner> &verifier,
    const std::weak_ptr<Option> &option
) : mVerifier(verifier), mOption(option) {
    spdlog::trace("Creating Fidgety::VerifierOptionLock.");
}

VerifierOptionLock::~VerifierOptionLock(void) {
    spdlog::trace("Deleting Fidgety::VerifierOptionLock.");
    if (!isReleased()) {
        std::shared_ptr<VerifierInner> verifier = mVerifier.lock();
        verifier->releaseLockOnDrop(std::move(*this));
        mVerifier.reset();
        mOption.reset();
    }
}

bool VerifierOptionLock::isReleased(void) const noexcept {
    return (mVerifier.expired() || mOption.expired());
}

ValidatorMessage VerifierOptionLock::release(void) {
    spdlog::trace("Trying to release Fidgety::VerifierOptionLock manually.");
    if (!isReleased()) {
        std::shared_ptr<VerifierInner> verifier = mVerifier.lock();
        ValidatorMessage message = verifier->releaseLock(std::move(*this));
        mVerifier.reset();
        mOption.reset();
        return message;
    } else {
        FIDGETY_CRITICAL(
            VerifierException,
            VerifierStatus::WeakPointerExpired,
            "Tried to release a lock when it has already been released."
        );
    }
}

bool VerifierOptionLock::optionExists(void) const {
    return !mOption.expired();
}

Option &VerifierOptionLock::getMutOption(void) {
    if (optionExists()) {
        return *mOption.lock();
    } else {
        FIDGETY_CRITICAL(
            VerifierException,
            VerifierStatus::WeakPointerExpired,
            "Fidgety::VerifierOptionLock::mOption no longer exists."
        );
    }
}

const Option &VerifierOptionLock::getOption(void) const {
    if (optionExists()) {
        return *mOption.lock();
    } else {
        FIDGETY_CRITICAL(
            VerifierException,
            VerifierStatus::WeakPointerExpired,
            "Fidgety::VerifierOptionLock::mOption no longer exists."
        );
    }
}

Verifier::Verifier(std::unique_ptr<ValidatorContextCreator> &&contextCreator) :
    mInner(new VerifierInner(std::move(contextCreator)))
{
    spdlog::trace("Creating Fidgety::Verifier with contextCreator.");
}

Verifier::Verifier(
    VerifierManagedOptionList &&options,
    std::unique_ptr<ValidatorContextCreator> &&contextCreator
) : mInner(new VerifierInner(std::move(options), std::move(contextCreator))) {
    spdlog::trace("Creating Fidgety::Verifier with options, contextCreator.");
}

Verifier::Verifier(const std::shared_ptr<VerifierInner> &inner) : mInner(inner) {
    spdlog::trace("Creating Fidgety::Verifier with inner.");
}

Verifier::Verifier(VerifierInner *inner) : mInner(inner) {
    spdlog::trace("Creating Fidgety::Verifier with inner.");
}

size_t Verifier::numberOfLocks(void) const {
    return mInner->numberOfLocks();
}

bool Verifier::optionExists(const OptionIdentifier &identifier) const {
    return mInner->optionExists(identifier);
}

bool Verifier::isOptionLocked(const OptionIdentifier &identifier) const {
    return mInner->isOptionLocked(identifier);
}

VerifierOptionLock Verifier::getLock(const OptionIdentifier &identifier) {
    spdlog::trace("Getting lock from Fidgety::Verifier.");
    std::weak_ptr<Option> option = mInner->lockOption(identifier);
    return VerifierOptionLock(mInner, option);
}

bool Verifier::canBeOverwritten(void) const {
    return mInner->getLocks().size() == 0;
}

VerifierStatus Verifier::overwriteOptions(void) {
    spdlog::trace("Clearing options in Fidgety::Verifier.");
    if (canBeOverwritten()) {
        mInner->getMutOptionList().clear();
        return VerifierStatus::Ok;
    } else {
        FIDGETY_ERROR(
            VerifierException,
            VerifierStatus::ResourceBusy,
            "Cannot overwrite the options in this Fidgety::Verifier."
        );
    }
}

VerifierStatus Verifier::overwriteOptions(VerifierManagedOptionList &&options) {
    spdlog::trace("Overwriting options with new VMOL in Fidgety::Verifier.");
    if (canBeOverwritten()) {
        mInner->getMutOptionList() = std::move(options);
        return VerifierStatus::Ok;
    } else {
        FIDGETY_ERROR(
            VerifierException,
            VerifierStatus::ResourceBusy,
            "Cannot overwrite the options in this Fidgety::Verifier."
        );
    }
}

VerifierStatus Verifier::purgeOrphanedOptions(void) {
    spdlog::debug("[Fidgety::Verifier::purgeOrphanedOptions] purging orphans");
    std::set<OptionIdentifier> orphans;
    return purgeOrphanedOptions(orphans);
}

VerifierStatus Verifier::purgeOrphanedOptions(const std::set<OptionIdentifier> &identifiers) {
    spdlog::debug(
        "[Fidgety::Verifier::purgeOrphanedOptions] purging options listed in 'identifiers'"
    );
    std::set<OptionIdentifier> orphans(identifiers);
    VerifierStatus status = mInner->findOrphanedOptions(orphans);
    if (status != VerifierStatus::Ok)
        return status;
    return mInner->purgeOrphanedOptions(orphans);
}

/*
#ifdef __cplusplus

    extern "C" {
        FIDGETY_ALLOC(FIDGETY_VCC_ALLOC_PROT(), ValidatorContextCreator);
        FIDGETY_DELETE(FIDGETY_VCC_DELETE_PROT());
    }

#endif
*/
