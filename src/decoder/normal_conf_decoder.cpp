/**
 * @file src/decoder/normal_conf_decoder.cpp
 * @author RenoirTan
 * @brief Implementation of the default decoder class
 * (Fidgety::NormalConfDecoder).
 * @version 0.1
 * @date 2022-02-17
 * 
 * @copyright Copyright (c) 2022
 */

#include <fmt/core.h>
#include <nlohmann/json.hpp>
#include "spdlog/spdlog.h"
#include <fidgety/decoder/normal_conf_decoder.hpp>
#include <fidgety/_utils.hpp>

using namespace Fidgety;

void NormalConfDecoder::dumpToIntermediate(void) {
    spdlog::trace("dumping NormalConfDecoder::mConfFile to NormalConfDecoder::mIntermediateFile");
    if (!isConfOpened() || !isIntermediateOpened()) {
        const std::string error_msg = fmt::format(
            "NormalConfDecoder::mConfFile and NormalConfDecoder::mIntermediateFile not open ({0})",
            (((uint8_t)isConfOpened()) << 1) | ((uint8_t)isIntermediateOpened())
        );
        spdlog::error(error_msg);
        throw DecoderException((int32_t) DecoderStatus::FilesNotOpen, error_msg);
    }
    spdlog::trace("NormalConfDecoder::mConfFile and NormalConfDecoder::mIntermediateFile opened");

    // PARSING PART

    nlohmann::json intermediate;
    size_t lineNo = 0;
    while (mConfFile.good()) {
        std::string line;
        std::getline(mConfFile, line);
        ++lineNo;
        
        size_t poundIndex = line.find('#');
        std::string noComment;
        if (poundIndex == std::string::npos) {
            noComment = line;
        } else {
            noComment = line.substr(0, poundIndex);
        }
        if (isEffectivelyEmpty(noComment)) {
            continue;
        }

        size_t equalsIndex = noComment.find('=');
        if (equalsIndex == std::string::npos) {
            const std::string error_msg = fmt::format("could not find '=' at line {0}", lineNo);
            spdlog::error(error_msg);
            DecoderException exception((int32_t) DecoderStatus::SyntaxError, error_msg);
            throw exception;
        }
        std::string key = noComment.substr(0, equalsIndex);
        std::string value = noComment.substr(equalsIndex + 1);
        trim(key); trim(value);
        
        if (key == "") {
            const std::string error_msg = fmt::format("no key before '=' at line {0}", lineNo);
            spdlog::error(error_msg);
            throw DecoderException((int32_t) DecoderStatus::SyntaxError, error_msg);
        }
        if (intermediate.contains(key)) {
            spdlog::warn(
                "{0} already set. However, the config file has another definition for {0} at {1}",
                key,
                lineNo
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
}
