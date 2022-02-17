/**
 * @file include/fidgety/_utils.hpp
 * @author RenoirTan
 * @brief Private (not really) header for utility functions used internally by
 * the component libraries in Fidgety.
 * @version 0.1
 * @date 2022-02-17
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef _FIDGETY_UTILS_HPP
#   define _FIDGETY_UTILS_HPP

#   include <string>

namespace Fidgety {
    void ltrim(std::string &s);
    void rtrim(std::string &s);
    void trim(std::string &s);
    bool isEffectivelyEmpty(const std::string &s);
}

#endif