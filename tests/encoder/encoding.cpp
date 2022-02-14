#include <iostream>
#include <string>
#include <fidgety/encoder.hpp>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include "spdlog/spdlog.h"

using namespace Fidgety;

#define CREATE_ENCODER(test_no)                                           \
    NormalConfEncoder encoder;                                            \
    std::ostringstream cs, is;                                            \
    cs << "../../../tmp/tests/encoder/test_" << test_no << ".conf";       \
    is << "../../../resources/tests/encoder/test_" << test_no << ".json"; \
    encoder.openConf(cs.str());                                           \
    encoder.openIntermediate(is.str());

bool filesEqual(const std::string &pathA, const std::string &pathB) {
    std::ifstream fileA(pathA), fileB(pathB);
    std::string lineA, lineB;
    // While not EOF for files A and B
    while (fileA.good() && fileB.good()) {
        std::getline(fileA, lineA); std::getline(fileB, lineB);
        if (lineA != lineB) {
            return false;
        }
    }
    // If either file is longer than the other, this expression will be false
    // It will never be the case where fileA and fileB will be bad good at the same time
    return fileA.good() == fileB.good();
}

TEST(EncoderEncoding, EmptyJson) {
    spdlog::set_level(spdlog::level::trace);
    //NormalConfEncoder encoder;
    //encoder.openConf("../../../tmp/tests/encoder/test_0.conf");
    //encoder.openIntermediate("../../../resources/tests/encoder/test_0.json");
    CREATE_ENCODER(0);
    ASSERT_TRUE(encoder.isConfOpened());
    ASSERT_TRUE(encoder.isIntermediateOpened());
    EncoderStatus status = encoder.dumpToConf();
    ASSERT_EQ(status, EncoderStatus::Ok);
}

TEST(EncoderEncoding, WellFormedJson) {
    spdlog::set_level(spdlog::level::trace);
    CREATE_ENCODER(1);
    ASSERT_TRUE(encoder.isConfOpened());
    ASSERT_TRUE(encoder.isIntermediateOpened());
    EncoderStatus status = encoder.dumpToConf();
    encoder.closeConf();
    encoder.closeIntermediate();
    ASSERT_TRUE(filesEqual(
        "../../../tmp/tests/encoder/test_1.conf",
        "../../../resources/tests/encoder/test_1_answer.conf"
    ));
    ASSERT_FALSE(filesEqual(
        "../../../tmp/tests/encoder/test_1.conf",
        "../../../resources/tests/encoder/empty"
    ));
}
