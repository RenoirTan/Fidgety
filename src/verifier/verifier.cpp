#include <random>
#include <set>
#include "spdlog/spdlog.h"
#include <fidgety/verifier.hpp>
#include <fidgety/_utils.hpp>

namespace Fidgety {
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
        VerifierIdentifier identifier;
        std::uniform_int_distribution<uint32_t> random;
        std::default_random_engine engine;
        for (size_t i = 0; i < identifier.size(); ++i) {
            identifier[i] = random(engine);
        }
        return identifier;
    }

    class VerifierInner {
        public:
            VerifierInner(ValidatorContextCreator &&contextCreator) :
                mContextCreator(contextCreator),
                mIdentifier(createIdentifier())
            { }

            VerifierInner(
                VerifierManagedOptionList &&options,
                ValidatorContextCreator &&contextCreator
            ) :
                mContextCreator(contextCreator),
                mIdentifier(createIdentifier()),
                mOptions(options)
            { }

            ~VerifierInner(void) { }

            VerifierInner(VerifierInner &&inner) = delete;
            VerifierInner(const VerifierInner &inner) = delete;
            VerifierInner &operator=(VerifierInner &&inner) = delete;
            VerifierInner &operator=(const VerifierInner &inner) = delete;

            bool optionExists(const OptionIdentifier &identifier) const {
                return mOptions.find(identifier) != mOptions.end();
            }

            bool isOptionLocked(const OptionIdentifier &identifier) const {
                return mLocks.find(identifier) == mLocks.end();
            }

            std::weak_ptr<Option> lockOption(const OptionIdentifier &identifier) {
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
                return std::weak_ptr<Option>(option->second);
            }

            ValidatorMessage releaseLock(VerifierOptionLock &&lock) {
                if (!lock.optionExists()) {
                    FIDGETY_CRITICAL(
                        VerifierException,
                        VerifierStatus::OptionDoesNotExist,
                        "Could not find option when releasing lock from Fidgety::VerifierInner."
                    );
                }
                Option &option = lock.getMutOption();
                const OptionIdentifier &identifier = option.getIdentifier();
                auto set_lock_it = mLocks.find(identifier);
                if (set_lock_it == mLocks.end()) {
                    spdlog::warn(
                        "Lock for '{0}' was not found in Fidgety::VerifierInner::mLocks.",
                        identifier
                    );
                }
                ValidatorContext context = mContextCreator.createContext(mOptions, identifier);
                ValidatorMessage message = option.validate(context);
                return message;
            }

            const ValidatorContextCreator &getContextCreator(void) const {
                return mContextCreator;
            }

            ValidatorContextCreator &getMutContextCreator(void) {
                return mContextCreator;
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

        protected:
            ValidatorContextCreator mContextCreator;
            VerifierIdentifier mIdentifier;
            VerifierManagedOptionList mOptions;
            std::set<OptionIdentifier> mLocks;
    };

    VerifierOptionLock::VerifierOptionLock(
        const std::weak_ptr<VerifierInner> &verifier,
        const std::weak_ptr<Option> &option
    ) : mVerifier(verifier), mOption(option) { }

    VerifierOptionLock::~VerifierOptionLock(void) {
        if (!mVerifier.expired()) {
            std::shared_ptr<VerifierInner> verifier = mVerifier.lock();
            verifier->releaseLock(std::move(*this));
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

    Verifier::Verifier(ValidatorContextCreator &&contextCreator) :
        mInner(new VerifierInner(std::move(contextCreator)))
    { }

    Verifier::Verifier(
        VerifierManagedOptionList &&options,
        ValidatorContextCreator &&contextCreator
    ) : mInner(new VerifierInner(std::move(options), std::move(contextCreator))) { }

    Verifier::Verifier(const std::shared_ptr<VerifierInner> &inner) : mInner(inner) { }

    Verifier::Verifier(VerifierInner *inner) : mInner(inner) { }

    bool Verifier::optionExists(const OptionIdentifier &identifier) const {
        return mInner->optionExists(identifier);
    }

    bool Verifier::isOptionLocked(const OptionIdentifier &identifier) const {
        return mInner->isOptionLocked(identifier);
    }

    VerifierOptionLock Verifier::getLock(const OptionIdentifier &identifier) {
        std::weak_ptr<Option> option = mInner->lockOption(identifier);
        return VerifierOptionLock(mInner, option);
    }

    bool Verifier::canBeOverwritten(void) const {
        return mInner->getLocks().size() == 0;
    }

    VerifierStatus Verifier::overwriteOptions(void) {
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

    VerifierStatus Verifier::overwriteOptions(const nlohmann::json &json) {
        FIDGETY_ERROR(
            VerifierException,
            VerifierStatus::Unimplemented,
            "Fidgety::Verifier::overwriteOptions "
            "cannot use nlohmann::json because it has not been implemented yet."
        );
    }
}