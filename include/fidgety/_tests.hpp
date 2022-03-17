/**
 * @file include/fidgety/_tests.hpp
 * @author RenoirTan
 * @brief Header file for the unit tests in Fidgety.
 * @version 0.1
 * @date 2022-03-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _FIDGETY_TESTS_HPP
#   define _FIDGETY_TESTS_HPP

#   include "spdlog/spdlog.h"
#   include <fidgety/config.h>

#   define _FIDGETY_SET_TESTLOGLEVEL() spdlog::set_level(spdlog::level::FIDGETY_TEST_LOGLEVEL)
#   define _FIDGETY_INIT_TEST()      \
    {                                \
        _FIDGETY_SET_TESTLOGLEVEL(); \
    }                                \

#endif
