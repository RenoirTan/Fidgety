/**
 * @file tests/selector/selector_encoder.cpp
 * @author RenoirTan
 * @brief Encoder test library for Selector.
 * @version 0.1
 * @date 2022-03-25
 * 
 * @copyright Copyright (c) 2022
 */

#include <fidgety/_tests.hpp>
#include <fidgety/_utils.hpp>
#include <fidgety/encoder.hpp>
#include <fidgety/extensions.hpp>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

using namespace Fidgety;

class Exp2Encoder : public Encoder {
    public:
        EncoderStatus dumpToConf(void) {
            spdlog::trace("dumping Exp2Encoder::mIntermediateFile to Exp2Encoder::mConfFile");

            if (!isConfOpened() || !isIntermediateOpened()) {
                FIDGETY_ERROR(
                    EncoderException,
                    EncoderStatus::FilesNotOpen,
                    "Exp2Encoder::mConfFile and Exp2Encoder::mIntermediateFile not open ({0})",
                    ((((uint8_t)isConfOpened()) << 1) | ((uint8_t)isIntermediateOpened()))
                );
            }

            spdlog::trace("[Exp2Encoder::dumpToConf] running through intermediate json");

            nlohmann::json intermediate = nlohmann::json::parse(mIntermediateFile);
            if (intermediate.type() != nlohmann::json::value_t::array) {
                FIDGETY_CRITICAL(
                    EncoderException,
                    EncoderStatus::VerifierError,
                    "[Exp2Encoder::dumpToConf] mIntermediateFile does not have a list"
                );
            }

            size_t linesWritten = 0;
            for (const auto &item : intermediate) {
                switch (item.type()) {
#define VALUE_TO_LINE(njvt, cxxvt) \
    case njvt: { \
        mConfFile << (cxxvt) item << '\n'; \
        ++linesWritten; \
    } \

                    VALUE_TO_LINE(nlohmann::json::value_t::string, std::string)
                    VALUE_TO_LINE(nlohmann::json::value_t::number_integer, int64_t)
                    VALUE_TO_LINE(nlohmann::json::value_t::number_unsigned, int64_t)

#undef VALUE_TO_LINE

                    default: {
                        FIDGETY_CRITICAL(
                            EncoderException,
                            EncoderStatus::InvalidDataType,
                            "[Exp2Encoder::dumpToConf] invalid data type '{0}' at line {1}",
                            item.type_name(),
                            linesWritten + 1
                        );
                    }
                }
            }

            spdlog::trace("[Exp2Encoder::dumpToConf] {0} lines written to mConfFile");
            mConfFile.flush();
            spdlog::debug("[Exp2Encoder::dumpToConf] success");
            return EncoderStatus::Ok;
        }
};

#ifdef __cplusplus

extern "C" {
    FIDGETY_ALLOC(FIDGETY_ENCODER_ALLOC_PROT(), Exp2Encoder);
    FIDGETY_DELETE(FIDGETY_ENCODER_DELETE_PROT());
}

#endif
