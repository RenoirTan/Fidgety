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

#   include <fidgety/exception.hpp>

namespace Fidgety {
    enum class VerifierStatus : int32_t {
        Ok = 0,
        FileNotFound = 1,
        CannotReadFile = 2,
        CannotWriteFile = 3,
        CannotCloseFile = 4,
        ResourceBusy = 5,
        CannotOpenMultipleFiles = 6,
        FilesNotOpen = 7,
        SyntaxError = 8
    };

    class VerifierException : public Exception {
        public:
            using Exception::Exception;
            std::string codeAsErrorType(void) const;

        protected:
            const char *getSimpleWhat(void) const noexcept;
    };

    class Verifier {
        Verifier(void) noexcept;
        ~Verifier(void);
    };
}

#endif
