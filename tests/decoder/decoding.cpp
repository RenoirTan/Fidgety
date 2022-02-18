#ifdef _WIN32
#   include <direct.h>
#   define getcwd _getcwd
#else
#   include <unistd.h>
#endif
#include <iostream>
#include <string>
#include <fidgety/decoder/normal_conf_decoder.hpp>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include "spdlog/spdlog.h"

using namespace Fidgety;

std::string presentWorkingDirectory(void) {
    char buffer[FILENAME_MAX]; // FILENAME_MAX is the maximum length of a file
    getcwd(buffer, FILENAME_MAX);
    std::string pwd(buffer);
    return pwd;
}

nlohmann::json loadJsonFromFile(const std::string &path) {
    spdlog::trace("opening json file '{0}'", path);
    std::ifstream handle(path, std::ifstream::in);
    spdlog::debug("is json file open? {0}", handle.is_open());
    auto object = nlohmann::json::parse(handle);
    handle.close();
    return object;
}

TEST(DecoderDecoding, DumpConf) {
    std::string pwd = presentWorkingDirectory();
    spdlog::set_level(spdlog::level::trace);
    spdlog::debug("present working directory: {0}", pwd);
    NormalConfDecoder decoder;
    decoder.openConf("../../../resources/tests/decoder/test_0.conf");
    decoder.openIntermediate("../../../tmp/tests/decoder/test_0.json");
    EXPECT_TRUE(decoder.isConfOpened());
    EXPECT_TRUE(decoder.isIntermediateOpened());
    DecoderStatus status = decoder.dumpToIntermediate();
    EXPECT_EQ(status, DecoderStatus::Ok);
    decoder.closeConf();
    decoder.closeIntermediate();
    spdlog::debug("opening answerKey");
    nlohmann::json answerKey = loadJsonFromFile(
        "../../../resources/tests/decoder/test_0_answer.json"
    );
    spdlog::debug("opening myAnswer");
    nlohmann::json myAnswer = loadJsonFromFile("../../../tmp/tests/decoder/test_0.json");
    EXPECT_EQ(myAnswer, answerKey);
}

TEST(DecoderDecoding, NoEqualsError) {
    NormalConfDecoder decoder;
    decoder.openConf("../../../resources/tests/decoder/test_1.conf");
    decoder.openIntermediate("../../../tmp/tests/decoder/test_1.json");
    EXPECT_TRUE(decoder.isConfOpened());
    EXPECT_TRUE(decoder.isIntermediateOpened());
    DecoderStatus status = decoder.dumpToIntermediate();
    EXPECT_EQ(status, DecoderStatus::SyntaxError);
}

TEST(DecoderDecoding, NoKeyError) {
    NormalConfDecoder decoder;
    decoder.openConf("../../../resources/tests/decoder/test_2.conf");
    decoder.openIntermediate("../../../tmp/tests/decoder/test_2.json");
    EXPECT_TRUE(decoder.isConfOpened());
    EXPECT_TRUE(decoder.isIntermediateOpened());
    DecoderStatus status = decoder.dumpToIntermediate();
    EXPECT_EQ(status, DecoderStatus::SyntaxError);
}
