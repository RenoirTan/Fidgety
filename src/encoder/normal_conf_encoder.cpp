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
    auto intermediate = nlohmann::json::parse(mIntermediateFile);
    if (intermediate.type() != json_value_t::object) {
        spdlog::error("NormalConfEncoder::mIntermediateFile is not a canonical JavaScript Object");
        return EncoderStatus::VerifierError;
    }
    size_t lines_read = 0;
    for (auto item = intermediate.cbegin(); item != intermediate.cend(); ++item) {
        json_value_t value_type = item->type();
        std::ostringstream line_stream;
        std::string key = item->key();
        switch (value_type) {
            case json_value_t::null:
                line_stream << "# " << key << "=";
                break;
            case json_value_t::boolean:
                bool bvalue = item->value();
                line_stream << key << "=" << bvalue ? "y" : "n";
                break;
            case json_value_t::string:
            case json_value_t::number_integer:
            case json_value_t::number_unsigned:
            case json_value_t::number_float:
            case json_value_t::binary:
                std::string svalue = item->value();
                line_stream << key << "=" << svalue;
                break;
            default:
                spdlog::error(
                    "Could not handle data type ({0}) of value whose key is '{1}'",
                    item->type_name(),
                    item->key()
                );
                return EncoderStatus::InvalidDataType;
        }
        std::string line = line_stream.str();
        mConfFile << line << std::endl;
    }

    mConfFile.flush();
    spdlog::debug(
        "successfully dumped NormalConfEncoder::mConfFile "
        "to NormalConfEncoder::mIntermediateFile"
    );
    return EncoderStatus::Ok;
}
