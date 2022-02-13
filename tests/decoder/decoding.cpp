#ifdef _WIN32
#   include <direct.h>
#   define getcwd _getcwd
#else
#   include <unistd.h>
#endif
#include <iostream>
#include <string>
#include <fidgety/decoder.hpp>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include "spdlog/spdlog.h"

using namespace Fidgety;

std::string present_working_directory(void) {
    char buffer[FILENAME_MAX]; // FILENAME_MAX is the maximum length of a file
    getcwd(buffer, FILENAME_MAX);
    std::string pwd(buffer);
    return pwd;
}

nlohmann::json load_json_from_file(const std::string &path) {
    spdlog::trace("opening json file '{0}'", path);
    std::ifstream handle(path, std::ifstream::in);
    spdlog::debug("is json file open? {0}", handle.is_open());
    auto object = nlohmann::json::parse(handle);
    handle.close();
    return object;
}

TEST(DecoderDecoding, DumpConf) {
    std::string pwd = present_working_directory();
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
    spdlog::debug("opening answer_key");
    nlohmann::json answer_key = load_json_from_file(
        "../../../resources/tests/decoder/test_0_answer.json"
    );
    spdlog::debug("opening my_answer");
    nlohmann::json my_answer = load_json_from_file("../../../tmp/tests/decoder/test_0.json");
    EXPECT_EQ(my_answer, answer_key);
}
