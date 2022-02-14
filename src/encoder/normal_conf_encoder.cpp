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
    size_t lines_written = 0;
    for (auto& item : intermediate.items()) {
        std::string original_key = item.key();
        std::string key = original_key;
        trim(key);
        if (key.empty()) {
            spdlog::warn("empty key found by NormalConfEncoder::dumpToConf");
        } else if (key != original_key) {
            spdlog::warn("whitespace found around the edges of this key: '{0}'", original_key);
        }
        auto value = item.value();
        json_value_t value_type = value.type();
        std::string value_type_name = value.type_name();
        std::ostringstream ss;
        switch (value_type) {
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
                    value_type_name,
                    original_key
                );
                return EncoderStatus::InvalidDataType;
            }
        }
        mConfFile << ss.str() << std::endl;
        ++lines_written;
    }

    spdlog::trace("{0} lines written to NormalConfEncoder::mConfFile", lines_written);
    mConfFile.flush();
    spdlog::debug(
        "successfully dumped NormalConfEncoder::mConfFile "
        "to NormalConfEncoder::mIntermediateFile"
    );
    return EncoderStatus::Ok;
}
