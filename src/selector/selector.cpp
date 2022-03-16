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
#include <fidgety/appdata.hpp>
#include <fidgety/selector.hpp>

using namespace Fidgety;

Selector::Selector(const Appdata &appdata) : mAppdata(appdata) { }

Selector::Selector(Appdata &&appdata) : mAppdata(std::move(appdata)) { }

bool Selector::isValid(void) const {
    return true;
}
