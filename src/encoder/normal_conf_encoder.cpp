#include <nlohmann/json.hpp>
#include "spdlog/spdlog.h"
#include <fidgety/encoder.hpp>
#include <fidgety/_utils.hpp>

using namespace Fidgety;
using json_value_t = nlohmann::detail::value_t;

EncoderStatus NormalConfEncoder::dumpToConf(void) {
    spdlog::trace("dumping NormalConfEncoder::mIntermediateFile to NormalConfEncoder::mConfFile");
    if (!isConfOpened() || !isIntermediateOpened()) {
        spdlog::error(
            "NormalConfEncoder::mConfFile and NormalConfEncoder::mIntermediateFile not open ({0})",
            (((uint8_t) isConfOpened()) << 1) | ((uint8_t) isIntermediateOpened())
        );
        return EncoderStatus::FilesNotOpen;
    }
    spdlog::trace("NormalConfEncoder::mConfFile and NormalConfEncoder::mIntermediateFile opened");

    // DUMPING PART
    nlohmann::json intermediate = nlohmann::json::parse(mIntermediateFile);
    if (intermediate.type() != json_value_t::object) {
        spdlog::error("NormalConfEncoder::mIntermediateFile is not a canonical JavaScript Object");
        return EncoderStatus::VerifierError;
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
        std::ostringstream ss;
        switch (valueType) {
            case json_value_t::boolean: {
                bool bvalue = (bool) value;
                ss << key << "=" << (bvalue ? "y" : "n");
                break;
            }
            case json_value_t::string:
            case json_value_t::number_float:
            case json_value_t::number_integer:
            case json_value_t::number_unsigned: {
                std::string svalue = (std::string) value;
                ss << key << "=" << svalue;
                break;
            }
            default: {
                spdlog::error(
                    "NormalConfEncoder does not accept data types of type {0} "
                    "encountered at key '{1}'",
                    valueTypeName,
                    originalKey
                );
                return EncoderStatus::InvalidDataType;
            }
        }
        mConfFile << ss.str() << std::endl;
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
