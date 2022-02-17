/**
 * @file src/private/string.cpp
 * @author your name (you@domain.com)
 * @brief Private string utilities.
 * @version 0.1
 * @date 2022-02-17
 * 
 * @copyright Copyright (c) 2022
 */

#include <algorithm>
#include <fidgety/_utils.hpp>

// https://stackoverflow.com/a/217605
// trim from start (in place)
void Fidgety::ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
void Fidgety::rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
void Fidgety::trim(std::string &s) {
    Fidgety::ltrim(s);
    Fidgety::rtrim(s);
}

// empty or all spaces
bool Fidgety::isEffectivelyEmpty(const std::string &s) {
    return (
        s.empty() ||
        std::all_of(s.begin(), s.end(), [](char c) { return std::isspace(c); })
    );
}
