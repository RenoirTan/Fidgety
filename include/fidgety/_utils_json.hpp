/**
 * @file include/fidgety/_utils_json.hpp
 * @author RenoirTan
 * @brief Utilities for JSON.
 * @version 0.1
 * @date 2022-03-19
 * 
 * @copyright Copyright (c) 2022
 */

#include <nlohmann/json.hpp>

namespace Fidgety {
    int32_t jsonScalarToString(const nlohmann::json &value, std::string &out);
};
