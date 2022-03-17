/**
 * @file tests/dylib/selector_decoder.cpp
 * @author RenoirTan
 * @brief Decoder test library for Selector.
 * @version 0.1
 * @date 2022-03-17
 * 
 * @copyright Copyright (c) 2022
 */

#include <cstdlib>
#include <vector>
#include <fidgety/_tests.hpp>
#include <fidgety/_utils.hpp>
#include <fidgety/decoder.hpp>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

using namespace Fidgety;

class Exp2Decoder : public virtual Decoder {
    DecoderStatus dumpToIntermediate(void) {
        spdlog::trace("dumping Exp2Decoder::mConfFile to Exp2Decoder::mIntermediateFile");

        if (!isConfOpened() || !isIntermediateOpened()) {
            FIDGETY_ERROR(
                DecoderException,
                DecoderStatus::FilesNotOpen,
                "Exp2Decoder::mConfFile and Exp2Decoder::mIntermediateFile not open ({0})",
                ((((uint8_t)isConfOpened()) << 1) | ((uint8_t)isIntermediateOpened()))
            );
        }

        spdlog::trace("running through conf file...");

        size_t lineNo = 0;
        std::vector<int32_t> exp2;
        while (mConfFile.good()) {
            std::string line;
            std::getline(mConfFile, line);
            ++lineNo;
            
            if (!isEffectivelyEmpty(line))
                exp2.push_back(std::atoi(line.c_str()));
        }

        spdlog::trace("generating intermediate");
        nlohmann::json intermediate;
        intermediate["exp2"] = exp2;

        spdlog::debug("writing intermediate");
        mIntermediateFile << intermediate;

        return DecoderStatus::Ok;
    }
};
