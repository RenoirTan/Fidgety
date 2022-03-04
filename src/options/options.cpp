/**
 * @file src/options/options.cpp
 * @author RenoirTan
 * @brief Implementation of options.
 * @version 0.1
 * @date 2022-03-04
 * 
 * @copyright Copyright (c) 2022
 */

#include "spdlog/spdlog.h"
#include <fidgety/options.hpp>
#include <fidgety/_utils.hpp>

using namespace Fidgety;

std::string OptionException::codeAsErrorType(void) const {
    switch (mCode) {
        case 0: return "Ok";
        case 1: return "InvalidValueType";
        case 2: return "IncompatibleOptionEditor";
        case 3: return "NotFound";
        default: return "Other";
    }
}

const char *OptionException::getSimpleWhat(void) const {
    return "A Fidgety::OptionException occurred.";
}
