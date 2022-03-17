/**
 * @file tests/decoder/decoding.cpp
 * @author RenoirTan
 * @brief Test suite for making sure Fidgety::NormalConfDecoder (and therefore
 * Fidgety::Decoder) works.
 * @version 0.1
 * @date 2022-03-14
 * 
 * @copyright Copyright (c) 2022
 */

#ifdef _WIN32
#   include <direct.h>
#   define getcwd _getcwd
#else
#   include <unistd.h>
#endif
#include <iostream>
#include <string>
#include <fidgety/_tests.hpp>
#include <fidgety/decoder/normal_conf_decoder.hpp>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

using namespace Fidgety;

std::string presentWorkingDirectory(void) {
    char buffer[FILENAME_MAX]; // FILENAME_MAX is the maximum length of a file
    getcwd(buffer, FILENAME_MAX);
    std::string pwd(buffer);
    return pwd;
}

nlohmann::json loadJsonFromFile(const std::string &path) {
    _FIDGETY_INIT_TEST();
    spdlog::trace("opening json file '{0}'", path);
    std::ifstream handle(path, std::ifstream::in);
    spdlog::debug("is json file open? {0}", handle.is_open());
    auto object = nlohmann::json::parse(handle);
    handle.close();
    return object;
}

TEST(DecoderDecoding, DumpConf) {
    _FIDGETY_INIT_TEST();
    std::string pwd = presentWorkingDirectory();
    spdlog::debug("present working directory: {0}", pwd);
    NormalConfDecoder decoder;
    ASSERT_EQ(decoder.openConf("../../../resources/tests/decoder/test_0.conf"), DecoderStatus::Ok);
    ASSERT_EQ(decoder.openIntermediate("../../../tmp/tests/decoder/test_0.json"), DecoderStatus::Ok);
    ASSERT_TRUE(decoder.isConfOpened());
    ASSERT_TRUE(decoder.isIntermediateOpened());
    ASSERT_EQ(decoder.dumpToIntermediate(), DecoderStatus::Ok);
    ASSERT_EQ(decoder.closeConf(), DecoderStatus::Ok);
    ASSERT_EQ(decoder.closeIntermediate(), DecoderStatus::Ok);
    spdlog::debug("opening answerKey");
    nlohmann::json answerKey = loadJsonFromFile(
        "../../../resources/tests/decoder/test_0_answer.json"
    );
    spdlog::debug("opening myAnswer");
    nlohmann::json myAnswer = loadJsonFromFile("../../../tmp/tests/decoder/test_0.json");
    ASSERT_EQ(myAnswer, answerKey);
}

TEST(DecoderDecoding, NoEqualsError) {
    _FIDGETY_INIT_TEST();
    NormalConfDecoder decoder;
    ASSERT_EQ(decoder.openConf("../../../resources/tests/decoder/test_1.conf"), DecoderStatus::Ok);
    ASSERT_EQ(decoder.openIntermediate("../../../tmp/tests/decoder/test_1.json"), DecoderStatus::Ok);
    ASSERT_TRUE(decoder.isConfOpened());
    ASSERT_TRUE(decoder.isIntermediateOpened());
    ASSERT_EQ(decoder.dumpToIntermediate(), DecoderStatus::SyntaxError);
}

TEST(DecoderDecoding, NoKeyError) {
    _FIDGETY_INIT_TEST();
    NormalConfDecoder decoder;
    ASSERT_EQ(decoder.openConf("../../../resources/tests/decoder/test_2.conf"), DecoderStatus::Ok);
    ASSERT_EQ(decoder.openIntermediate("../../../tmp/tests/decoder/test_2.json"), DecoderStatus::Ok);
    ASSERT_TRUE(decoder.isConfOpened());
    ASSERT_TRUE(decoder.isIntermediateOpened());
    ASSERT_EQ(decoder.dumpToIntermediate(), DecoderStatus::SyntaxError);
}
