/**
 * @file include/fidgety/_general.hpp
 * @author RenoirTan
 * @brief Header file for libraries and applications.
 * @version 0.1
 * @date 2022-04-29
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef _FIDGETY_GENERAL_HPP
#   define _FIDGETY_GENERAL_HPP

#   define _FIDGETY_SET_APPLOGLEVEL() spdlog::set_level(spdlog::level::FIDGETY_APP_LOGLEVEL)
#   define _FIDGETY_SET_APPLOGFORMAT() \
    spdlog::set_pattern("[source %s] [function %!] [line %#] %v")

#   define _FIDGETY_INIT_APP() {    \
        _FIDGETY_SET_APPLOGLEVEL(); \
    }                               \

#endif
