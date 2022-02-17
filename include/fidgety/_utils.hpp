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