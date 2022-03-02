/**
 * @file src/encoder/normal_conf_encoder.cpp
 * @author RenoirTan
 * @brief Implementation of the default encoder class
 * (Fidgety::NormalConfEncoder).
 * @version 0.1
 * @date 2022-02-17
 * 
 * @copyright Copyright (c) 2022
 */

#include <fmt/core.h>
#include <nlohmann/json.hpp>
#include "spdlog/spdlog.h"
#include <fidgety/encoder/normal_conf_encoder.hpp>
#include <fidgety/_utils.hpp>

using namespace Fidgety;
using json_value_t = nlohmann::detail::value_t;

EncoderStatus NormalConfEncoder::dumpToConf(void) {
    spdlog::trace("dumping NormalConfEncoder::mIntermediateFile to NormalConfEncoder::mConfFile");
    if (!isConfOpened() || !isIntermediateOpened()) {
        FIDGETY_ERROR(
            EncoderException,
            EncoderStatus::FileNotFound,
            "NormalConfEncoder::mConfFile and NormalConfEncoder::mIntermediateFile not open ({0})",
            ((((uint8_t)isConfOpened()) << 1) | ((uint8_t)isIntermediateOpened()))
        );
    }
    spdlog::trace("NormalConfEncoder::mConfFile and NormalConfEncoder::mIntermediateFile opened");

    // DUMPING PART
    nlohmann::json intermediate = nlohmann::json::parse(mIntermediateFile);
    if (intermediate.type() != json_value_t::object) {
        FIDGETY_CRITICAL(
            EncoderException,
            EncoderStatus::VerifierError,
            "NormalConfEncoder::mIntermediateFile is not a canonical JavaScript Object"
        );
    }
    size_t linesWritten = 0;
    for (auto& item : intermediate.items()) {
        std::string originalKey = item.key();
        std::string key = originalKey;
        trim(key);
        if (key.empty()) {
            spdlog::warn("empty key found by NormalConfEncoder::dumpToConf");
        } else if (key != originalKey) {
            spdlog::warn("whitespace found around the edges of this key: '{0}'", originalKey);
        }
        auto value = item.value();
        json_value_t valueType = value.type();
        std::string valueTypeName = value.type_name();
        std::string line;
        switch (valueType) {
            case json_value_t::boolean: {
                bool bvalue = (bool) value;
                line = fmt::format("{0}={1}", key, bvalue ? "y" : "n");
                break;
            }
#define VALUE_TO_LINE(datatype) {                     \
    datatype forced_value = (datatype) value;         \
    line = fmt::format("{0}={1}", key, forced_value); \
    break;                                            \
}
            case json_value_t::string: VALUE_TO_LINE(std::string)
            case json_value_t::number_float: VALUE_TO_LINE(std::double_t)
            case json_value_t::number_integer: VALUE_TO_LINE(std::int64_t)
            case json_value_t::number_unsigned: VALUE_TO_LINE(std::uint64_t)
#undef VALUE_TO_LINE
            default: {
                FIDGETY_CRITICAL(
                    EncoderException,
                    EncoderStatus::InvalidDataType,
                    "NormalConfEncoder does not accept data types of type {0} "
                    "encountered at key '{1}'",
                    valueTypeName,
                    originalKey
                );
            }
        }
        mConfFile << line << std::endl;
        ++linesWritten;
    }

    spdlog::trace("{0} lines written to NormalConfEncoder::mConfFile", linesWritten);
    mConfFile.flush();
    spdlog::debug(
        "successfully dumped NormalConfEncoder::mConfFile "
        "to NormalConfEncoder::mIntermediateFile"
    );
    return EncoderStatus::Ok;
}
