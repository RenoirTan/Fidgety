#include <nlohmann/json.hpp>
#include "spdlog/spdlog.h"
#include <fidgety/decoder.hpp>
#include <fidgety/_utils.hpp>

using namespace Fidgety;

DecoderStatus NormalConfDecoder::dumpToIntermediate(void) {
    spdlog::trace("dumping NormalConfDecoder::mConfFile to NormalConfDecoder::mIntermediateFile");
    if (!isConfOpened() || !isIntermediateOpened()) {
        spdlog::error(
            "NormalConfDecoder::mConfFile and NormalConfDecoder::mIntermediateFile not open ({0})",
            (((uint8_t) isConfOpened()) << 1) | ((uint8_t) isIntermediateOpened())
        );
        return DecoderStatus::FilesNotOpen;
    }
    spdlog::trace("NormalConfDecoder::mConfFile and NormalConfDecoder::mIntermediateFile opened");

    // PARSING PART

    nlohmann::json intermediate;
    size_t line_no = 0;
    while (mConfFile.good()) {
        std::string line;
        std::getline(mConfFile, line);
        ++line_no;
        
        size_t pound_index = line.find('#');
        std::string no_comment;
        if (pound_index == std::string::npos) {
            no_comment = line;
        } else {
            no_comment = line.substr(0, pound_index);
        }
        if (is_effectively_empty(no_comment)) {
            continue;
        }

        size_t equals_index = no_comment.find('=');
        if (equals_index == std::string::npos) {
            spdlog::error("could not find '=' at line {0}", line_no);
            return DecoderStatus::SyntaxError;
        }
        std::string key = no_comment.substr(0, equals_index);
        std::string value = no_comment.substr(equals_index + 1);
        trim(key); trim(value);
        
        if (key == "") {
            spdlog::error("no key before '=' at line {0}", line_no);
            return DecoderStatus::SyntaxError;
        }
        if (intermediate.contains(key)) {
            spdlog::warn(
                "{0} already set. However, the config file has another definition for {0} at {1}",
                key,
                line_no
            );
            spdlog::warn("overriding previous value of {0}", key);
        }
        intermediate[key] = value;
    }

    mIntermediateFile << intermediate;
    spdlog::debug(
        "successfully dumped NormalConfDecoder::mConfFile "
        "to NormalConfDecoder::mIntermediateFile"
    );
    return DecoderStatus::Ok;
}
