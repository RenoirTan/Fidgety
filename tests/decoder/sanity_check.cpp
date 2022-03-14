/**
 * @file tests/decoder/sanity_check.cpp
 * @author RenoirTan
 * @brief Test suite for making sure that Fidgety::Decoder doesn't do anything
 * stupid.
 * @version 0.1
 * @date 2022-03-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <fidgety/decoder/normal_conf_decoder.hpp>
#include <gtest/gtest.h>

using namespace Fidgety;

TEST(DecoderSanityCheck, ClosedFiles) {
    NormalConfDecoder decoder;
    EXPECT_FALSE(decoder.isConfOpened());
    EXPECT_FALSE(decoder.isIntermediateOpened());
}
