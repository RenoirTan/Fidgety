/**
 * @file include/fidgety/options/_option_exception.hpp
 * @author RenoirTan
 * @brief Declaration of the exception class for Fidgety::Options and its
 * supporting functions.
 * @version 0.1
 * @date 2022-04-15
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef _FIDGETY_OPTIONS_OPTION_EXCEPTION_HPP
#   define _FIDGETY_OPTIONS_OPTION_EXCEPTION_HPP

#   include "_fwd.hpp"
#   include <fidgety/exception.hpp>

namespace Fidgety {
    enum class OptionStatus : int32_t {
        Ok = 0,
        InvalidValueType = 1,
        IncompatibleOptionEditor = 2,
        NotFound = 3,
        InvalidIdentifier = 4,
        InvalidName = 5
    };

    class OptionException : public Exception {
        public:
            using Exception::Exception;
            std::string codeAsErrorType(void) const;

        protected:
            const char *getSimpleWhat(void) const noexcept;
    };
}

#endif
