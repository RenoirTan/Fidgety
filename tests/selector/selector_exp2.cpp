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

#include <fidgety/_tests.hpp>
#include <fidgety/_utils.hpp>
#include <fidgety/appdata.hpp>
#include <fidgety/decoder.hpp>
#include <fidgety/encoder.hpp>
#include <fidgety/options.hpp>
#include <fidgety/selector.hpp>
#include <fidgety/verifier.hpp>
#include <gtest/gtest.h>
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

TEST(SelectorExp2, Selector) {
    _FIDGETY_INIT_TEST();
    Selector selector(_makeAppdata());
    ASSERT_TRUE(selector.isValid());
    ASSERT_EQ(selector.processHints(), SelectorStatus::Ok);
    
    std::string decoderLocation = selector.getDecoderLocation();
    std::string encoderLocation = selector.getEncoderLocation();
    std::string validatorLocation = selector.getValidatorLocation();
    std::string vccLocation = selector.getValidatorContextCreatorLocation();
    EXPECT_EQ(decoderLocation, "./libselector_decoder.so");
    EXPECT_EQ(encoderLocation, "./libselector_encoder.so");
    EXPECT_EQ(validatorLocation, "./libselector_validator.so");
    EXPECT_EQ(vccLocation, "./libselector_vcc.so");
}