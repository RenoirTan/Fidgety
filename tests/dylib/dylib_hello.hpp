/**
 * @file tests/dylib/dylib_hello.hpp
 * @author RenoirTan
 * @brief Common declarations for the dylib_hello test suite.
 * @version 0.1
 * @date 2022-03-16
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef _DYLIB_HPP
#   define _DYLIB_HPP

#   include <string>
#   include "spdlog/spdlog.h"

class Hello {
    public:
        Hello(const std::string &name) : mName(name) { }

        ~Hello(void) {
            spdlog::info("Deleting Hello");
        }

        virtual std::string greet(void) = 0;
    
    protected:
        std::string mName;
};

#endif
