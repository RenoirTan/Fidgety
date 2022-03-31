/**
 * @file tests/selector/selector_exp2.cpp
 * @author RenoirTan
 * @brief Selector test by simulating an app that requires a text file which
 * must have a consecutive sequence of integers that are powers of 2, separated
 * by newlines.
 * @version 0.1
 * @date 2022-03-25
 * 
 * @copyright Copyright (c) 2022
 */

#include <fstream>
#include <fidgety/_tests.hpp>
#include <fidgety/_utils.hpp>
#include <fidgety/appdata.hpp>
#include <fidgety/decoder.hpp>
#include <fidgety/encoder.hpp>
#include <fidgety/options.hpp>
#include <fidgety/selector.hpp>
#include <fidgety/verifier.hpp>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

using namespace Fidgety;

static inline Appdata _makeAppdata(void) {
    Appdata appdata {
        .configFilePath = "../../../resources/tests/selector/test_0.conf",
        .appName = "selector",
        .searchPaths{"."},
        .loadablePartsFileNames{
            .decoder{"libselector_decoder.so"},
            .encoder{"libselector_encoder.so"},
            .validator{"libselector_validator.so"},
            .validatorContextCreator{"libselector_vcc.so"}
        }
    };
    return appdata;
}

#define _SELECTOR_TEST() \
    Appdata appdata = _makeAppdata(); \
    Selector selector(appdata); \
    ASSERT_TRUE(selector.isValid()); \
    ASSERT_EQ(selector.processHints(), SelectorStatus::Ok); \
    std::string decoderLocation = selector.getDecoderLocation(); \
    std::string encoderLocation = selector.getEncoderLocation(); \
    std::string validatorLocation = selector.getValidatorLocation(); \
    std::string vccLocation = selector.getValidatorContextCreatorLocation(); \
    EXPECT_EQ(decoderLocation, "./libselector_decoder.so"); \
    EXPECT_EQ(encoderLocation, "./libselector_encoder.so"); \
    EXPECT_EQ(validatorLocation, "./libselector_validator.so"); \
    EXPECT_EQ(vccLocation, "./libselector_vcc.so"); \

TEST(SelectorExp2, Selector) {
    _FIDGETY_INIT_TEST();
    _SELECTOR_TEST();
}

TEST(SelectorExp2, Loader) {
#define INTERMEDIATE_FILE "../../../tmp/tests/selector/test_0.json"

    _FIDGETY_INIT_TEST();
    _SELECTOR_TEST();

    Loader loader(std::move(selector));
    ASSERT_EQ(loader.lateInit(), SelectorStatus::Ok);

    ASSERT_EQ(loader.openDecoder(), DylibStatus::Ok);
    ASSERT_EQ(loader.openEncoder(), DylibStatus::Ok);
    ASSERT_EQ(loader.openValidator(), DylibStatus::Ok);
    ASSERT_EQ(loader.openValidatorContextCreator(), DylibStatus::Ok);

    DyclassBox<Decoder> decoder = loader.getDecoder();
    DyclassBox<Encoder> encoder = loader.getEncoder();
    DyclassBox<Validator> validator = loader.getValidator();
    DyclassBox<ValidatorContextCreator> vcc = loader.getValidatorContextCreator();

    ASSERT_EQ(decoder->openConf(appdata.configFilePath), DecoderStatus::Ok);
    ASSERT_EQ(decoder->openIntermediate(INTERMEDIATE_FILE), DecoderStatus::Ok);
    ASSERT_EQ(decoder->dumpToIntermediate(), DecoderStatus::Ok);
    ASSERT_EQ(decoder->closeConf(), DecoderStatus::Ok);
    ASSERT_EQ(decoder->closeIntermediate(), DecoderStatus::Ok);

    nlohmann::json intermediate;
    {
        std::ifstream intermediateFile;
        intermediateFile.open(INTERMEDIATE_FILE);
        intermediateFile >> intermediate;
    }
    VerifierManagedOptionList vmol;
    size_t index = 0;
    for (const auto &value : intermediate["exp2"]) {
        std::string identifier = fmt::format("{}", index);
        spdlog::trace("[SelectorExp2_Loader] creating option with identifier: {}", identifier);
        OptionEditor oe(OptionEditorType::TextEntry, std::map<std::string, std::string>());
        std::unique_ptr<Validator> myValidator(validator->clone());
        int32_t avt = OptionValueType::RAW_VALUE;
        OptionValue ovalue(fmt::format("{}", (std::int64_t) value), avt);
        auto option = std::make_shared<Option>(
            std::string(identifier),
            std::move(oe),
            std::move(myValidator),
            std::move(ovalue)
        );
        vmol[identifier] = option;
        ++index;
    }

    Verifier verifier(
        std::move(vmol),
        std::unique_ptr<ValidatorContextCreator>(new ValidatorContextCreator(*vcc))
    );
    {
        ASSERT_FALSE(verifier.isOptionLocked("1"));
        auto vLock = verifier.getLock("1");
        ASSERT_TRUE(vLock.optionExists());
        {
            const Option &option = vLock.getOption();
            EXPECT_EQ(option.getRawValue(), "2");
        }
        ValidatorMessage vMessage = vLock.release();
        EXPECT_EQ(vMessage.getMessageType(), ValidatorMessageType::Valid);
    }

    // TODO: Add part that overrides intermediate json file with new options list

    ASSERT_EQ(encoder->openConf("../../../tmp/tests/selector/test_0.conf"), EncoderStatus::Ok);
    ASSERT_EQ(encoder->openIntermediate(INTERMEDIATE_FILE), EncoderStatus::Ok);
    ASSERT_EQ(encoder->dumpToConf(), EncoderStatus::Ok);
    ASSERT_EQ(encoder->closeConf(), EncoderStatus::Ok);
    ASSERT_EQ(encoder->closeIntermediate(), EncoderStatus::Ok);

    ASSERT_EQ(loader.closeDecoder(), DylibStatus::Ok);
    ASSERT_EQ(loader.closeEncoder(), DylibStatus::Ok);
    ASSERT_EQ(loader.closeValidator(), DylibStatus::Ok);
    ASSERT_EQ(loader.closeValidatorContextCreator(), DylibStatus::Ok);

#undef INTERMEDIATE_FILE
}
