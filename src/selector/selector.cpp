/**
 * @file src/selector/selector.cpp
 * @author RenoirTan
 * @brief Source file for Fidgety's "selectors"
 * @version 0.1
 * @date 2022-03-15
 * 
 * @copyright Copyright (c) 2022
 */

#include <QFile>
#include "spdlog/spdlog.h"
#include <fidgety/appdata.hpp>
#include <fidgety/selector.hpp>

using namespace Fidgety;

Selector::Selector(const Appdata &appdata) : mAppdata(appdata) { }

Selector::Selector(Appdata &&appdata) : mAppdata(std::move(appdata)) { }

bool Selector::isValid(void) const {
// Check if LoadablePartsFileNames is non-zero (not empty)
#define CHK_LPFN_NZ(part)                                   \
    if (mAppdata.loadablePartsFileNames.part.size() == 0) { \
        return false;                                       \
    }                                                       \

    CHK_LPFN_NZ(decoder);
    CHK_LPFN_NZ(encoder);
    CHK_LPFN_NZ(validator);
    CHK_LPFN_NZ(validatorContextCreator);
#undef CHK_LPFN_NZ

    return true;
}
