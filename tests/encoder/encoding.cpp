/**
 * @file tests/encoder/encoding.cpp
 * @author RenoirTan
 * @brief Test suite for making sure that Fidgety::NormalConfEncoder (and by
 * extension Fidgety::Encoder) works.
 * @version 0.1
 * @date 2022-03-14
 * 
 * @copyright Copyright (c) 2022
 */

#include <iostream>
#include <string>
#include <fidgety/_tests.hpp>
#include <fidgety/encoder/normal_conf_encoder.hpp>
#include <fmt/core.h>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

using namespace Fidgety;

#define CREATE_ENCODER(testNo)                                                                    \
    NormalConfEncoder encoder;                                                                    \
    const std::string cs = fmt::format("../../../tmp/tests/encoder/test_{0}.conf", testNo);       \
    const std::string is = fmt::format("../../../resources/tests/encoder/test_{0}.json", testNo); \
    EXPECT_EQ(encoder.openConf(cs), EncoderStatus::Ok);                                           \
    EXPECT_EQ(encoder.openIntermediate(is), EncoderStatus::Ok);

TEST(EncoderEncoding, EmptyJson) {
    _FIDGETY_INIT_TEST();
    //NormalConfEncoder encoder;
    //encoder.openConf("../../../tmp/tests/encoder/test_0.conf");
    //encoder.openIntermediate("../../../resources/tests/encoder/test_0.json");
    CREATE_ENCODER(0);
    ASSERT_TRUE(encoder.isConfOpened());
    ASSERT_TRUE(encoder.isIntermediateOpened());
    ASSERT_EQ(encoder.dumpToConf(), EncoderStatus::Ok);
}

TEST(EncoderEncoding, WellFormedJson) {
    _FIDGETY_INIT_TEST();
    CREATE_ENCODER(1);
    ASSERT_TRUE(encoder.isConfOpened());
    ASSERT_TRUE(encoder.isIntermediateOpened());
    ASSERT_EQ(encoder.dumpToConf(), EncoderStatus::Ok);
    ASSERT_EQ(encoder.closeConf(), EncoderStatus::Ok);
    ASSERT_EQ(encoder.closeIntermediate(), EncoderStatus::Ok);
    ASSERT_TRUE(filesEqual(
        "../../../tmp/tests/encoder/test_1.conf",
        "../../../resources/tests/encoder/test_1_answer.conf"
    ));
    ASSERT_FALSE(filesEqual(
        "../../../tmp/tests/encoder/test_1.conf",
        "../../../resources/tests/encoder/empty"
    ));
}
