/**
 * @file src/private/json.cpp
 * @author RenoirTan
 * @brief Source file for JSON utilities.
 * @version 0.1
 * @date 2022-03-19
 * 
 * @copyright Copyright (c) 2022
 */

#include <fidgety/config.h>

#include <fmt/core.h>
#include <nlohmann/json.hpp>
#include <fidgety/_utils_json.hpp>

using namespace Fidgety;

int32_t Fidgety::jsonScalarToString(const nlohmann::json &value, std::string &out) {
    using nj_value_t = nlohmann::json::value_t;

    nj_value_t valueType = value.type();
    switch (valueType) {
#define SCALAR_TO_STRING(njvt, cxxvt) case njvt: {  \
    cxxvt cxx_value = (cxxvt) value;                \
    out = std::move(fmt::format("{0}", cxx_value)); \
    break;                                          \
}                                                   \

    SCALAR_TO_STRING(nj_value_t::string, std::string)
    SCALAR_TO_STRING(nj_value_t::number_integer, int64_t)
    SCALAR_TO_STRING(nj_value_t::number_unsigned, uint64_t)
    SCALAR_TO_STRING(nj_value_t::number_float, double)
    case nj_value_t::null: {
        out = "";
        break;
    }
    case nj_value_t::boolean: {
        out = (bool) value ? "yes" : "no";
        break;
    }
    default:
        return 1;

#undef SCALAR_TO_STRING
    }

    return 0;
}
