/**
 * @file tests/dylib/dylib_hello_lib.cpp
 * @author RenoirTan
 * @brief Library for the class to be loaded by dylib_hello.cpp
 * @version 0.1
 * @date 2022-03-16
 * 
 * @copyright Copyright (c) 2022
 */

#include <iostream>
#include <fidgety/dylib.hpp>
#include <fmt/core.h>
#include "dylib_hello.hpp"

class EnglishHello : public virtual Hello {
    public:
        using Hello::Hello;

        std::string greet(void) {
            return fmt::format("Hello, my name is {0}", mName);
        }
};

extern "C" {
    EnglishHello *allocator(void) {
        return new EnglishHello("Renoir");
    }

    void deleter(EnglishHello *hello) {
        delete hello;
    }
}
