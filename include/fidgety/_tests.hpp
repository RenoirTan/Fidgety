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

#   include <fstream>
#   include <string>
#   include <spdlog/spdlog.h>
#   include <fidgety/config.h>

#   define _FIDGETY_SET_TESTLOGLEVEL() spdlog::set_level(spdlog::level::FIDGETY_TEST_LOGLEVEL)
#   define _FIDGETY_SET_TESTLOGFORMAT() \
    spdlog::set_pattern("[source %s] [function %!] [line %#] %v")

#   define _FIDGETY_INIT_TEST()       \
    {                                 \
        _FIDGETY_SET_TESTLOGLEVEL();  \
    }                                 \

bool filesEqual(const std::string &pathA, const std::string &pathB) {
    std::ifstream fileA(pathA), fileB(pathB);
    std::string lineA, lineB;
    // While not EOF for files A and B
    while (fileA.good() && fileB.good()) {
        std::getline(fileA, lineA); std::getline(fileB, lineB);
        if (lineA != lineB) {
            return false;
        }
    }
    // If either file is longer than the other, this expression will be false
    // It will never be the case where fileA and fileB will be good at the same time
    return fileA.good() == fileB.good();
}

#endif
