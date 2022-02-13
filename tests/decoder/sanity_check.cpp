#include <fidgety/decoder.hpp>
#include <gtest/gtest.h>

using namespace Fidgety;

TEST(DecoderSanityCheck, ClosedFiles) {
    NormalConfDecoder decoder;
    EXPECT_FALSE(decoder.isConfOpened());
    EXPECT_FALSE(decoder.isIntermediateOpened());
}
