/**
 * @file include/fidgety/database.hpp
 * @author RenoirTan
 * @brief Header file for managing the data provided by developers on their
 * applications. This includes the default values of different settings, what
 * decoders/encoders to use and where to find them, etc.
 * @version 0.1
 * @date 2022-03-18
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef FIDGETY_DATABASE_HPP
#   define FIDGETY_DATABASE_HPP

#   include <fidgety/exception.hpp>
#   include <fidgety/options.hpp>
#   include <fidgety/verifier.hpp>

namespace Fidgety {
    enum class DatabaseStatus : int32_t {
        Ok = 0,
        FileNotFound = 1,
        CannotReadFile = 2,
        CannotWriteFile = 3,
        CannotCloseFile = 4,
        ResourceBusy = 5,
        CannotOpenMultipleFiles = 6,
        FilesNotOpen = 7,
        SyntaxError = 8,
        InvalidData = 9,
        MultipleCandidates = 10,
        NoCandidates = 11
    };

    class DatabaseException : public Exception {
        public:
            using Exception::Exception;
            std::string codeAsErrorType(void) const;
        
        protected:
            const char *getSimpleWhat(void) const noexcept;
    };
}

#endif
