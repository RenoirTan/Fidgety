/**
 * @file include/fidgety/exception.hpp
 * @author RenoirTan
 * @brief Header file for the base exception class used in Fidgety, from which
 * all other Fidgety exception classes are derived from.
 * @version 0.1
 * @date 2022-02-20
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef FIDGETY_EXCEPTION_HPP
#   define FIDGETY_EXCEPTION_HPP

#   include <cstdint>
#   include <exception>
#   include <string>

namespace Fidgety {
    class Exception : public std::exception {
        public:
            // overriding std::exception
            Exception(void) noexcept;
            Exception(const std::exception &origin) noexcept;
            Exception(const Exception &origin) noexcept;
            Exception &operator=(const std::exception &origin) noexcept;
            Exception &operator=(const Exception &origin) noexcept;
            ~Exception(void);
            const char *what(void) const noexcept;

            Exception(
                const int32_t code,
                const std::string &info,
                const std::string &supportingInfo
            );
            Exception(const int32_t code, const std::string &info);
            Exception(const int32_t code, std::string &&info, std::string &&supportingInfo);
            Exception(const int32_t code, std::string &&info);
            Exception(const int32_t code);

            int32_t getCode(void) const;
            virtual std::string codeAsErrorType(void) const;
            const std::string &getInformation(void) const;
            const std::string &getSupportingInformation(void) const;

            std::string getShortDescription(void) const;
            std::string getGenericDescription(void) const;
            std::string getLongDescription(void) const;

            void exit(bool printToStdErr=true, bool outputToLog=true);
        
        protected:
            int32_t mCode;
            std::string mInformation;
            std::string mSupportingInformation;
    };
}

#endif
