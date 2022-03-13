/**
 * @file include/fidgety/verifier.hpp
 * @author RenoirTan
 * @brief Header file for the base verifier and any associated declarations.
 * @version 0.1
 * @date 2022-03-02
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef FIDGETY_VERIFIER_HPP
#   define FIDGETY_VERIFIER_HPP

#   include <map>
#   include <memory>
#   include <nlohmann/json.hpp>
#   include <fidgety/exception.hpp>
#   include <fidgety/options.hpp>

namespace Fidgety {

    // BEGIN FORWARD DECLARATIONS

    class ValidatorContextCreator;
    enum class VerifierStatus;
    class VerifierException;
    class VerifierOptionLock;
    class VerifierInner;
    class Verifier;

    // END FORWARD DECLARATIONS

    typedef std::map<OptionIdentifier, std::shared_ptr<Option>> VerifierManagedOptionList;
    typedef std::array<uint32_t, 16> VerifierIdentifier;

    class ValidatorContextCreator {
        public:
            ValidatorContext createContext(
                const VerifierManagedOptionList &verifier,
                const OptionIdentifier &identifier
            );
    };

    enum class VerifierStatus : int32_t {
        Ok = 0,
        FileNotFound = 1,
        CannotReadFile = 2,
        CannotWriteFile = 3,
        CannotCloseFile = 4,
        ResourceBusy = 5,
        CannotOpenMultipleFiles = 6,
        FilesNotOpen = 7,
        SyntaxError = 8,
        WeakPointerExpired = 9,
        OptionDoesNotExist = 10,
        Unimplemented = 11
    };

    class VerifierException : public Exception {
        public:
            using Exception::Exception;
            std::string codeAsErrorType(void) const;

        protected:
            const char *getSimpleWhat(void) const noexcept;
    };

    class VerifierOptionLock {
        public:
            VerifierOptionLock(
                const std::weak_ptr<VerifierInner> &verifier,
                const std::weak_ptr<Option> &option
            );
            ~VerifierOptionLock(void);

            VerifierOptionLock(const VerifierOptionLock &lock) = delete;
            VerifierOptionLock(VerifierOptionLock &&lock) = default;
            VerifierOptionLock &operator=(const VerifierOptionLock &lock) = delete;
            VerifierOptionLock &operator=(VerifierOptionLock &&lock) = default;

            bool optionExists(void) const;
            Option &getMutOption(void);
            const Option &getOption(void) const;

        protected:
            std::weak_ptr<Option> mOption;
            std::weak_ptr<VerifierInner> mVerifier;
    };

    class Verifier {
        public:
            Verifier(ValidatorContextCreator &&contextCreator);
            Verifier(VerifierManagedOptionList &&options, ValidatorContextCreator &&contextCreator);
            ~Verifier(void) = default;

            Verifier(const std::shared_ptr<VerifierInner> &inner);
            Verifier(VerifierInner *inner);

            Verifier(Verifier &&verifier) = default;
            Verifier(const Verifier &verifier) = delete;
            Verifier &operator=(Verifier &&verifier) = default;
            Verifier &operator=(const Verifier &verifier) = delete;

            bool optionExists(const OptionIdentifier &identifier) const;
            bool isOptionLocked(const OptionIdentifier &identifier) const;
            VerifierOptionLock getLock(const OptionIdentifier &identifier);

            bool canBeOverwritten(void) const;
            VerifierStatus overwriteOptions(void);
            VerifierStatus overwriteOptions(VerifierManagedOptionList &&options);
            virtual VerifierStatus overwriteOptions(const nlohmann::json &json);

        protected:
            std::shared_ptr<VerifierInner> mInner;
    };
}

#endif
