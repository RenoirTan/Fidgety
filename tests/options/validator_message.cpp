/**
 * @file tests/options/validator_message.cpp
 * @author RenoirTan
 * @brief Test suite for Fidgety::ValidatorMessage.
 * @version 0.1
 * @date 2022-03-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <iostream>
#include <string>
#include <fidgety/options.hpp>
#include <gtest/gtest.h>
#include "spdlog/spdlog.h"

using namespace Fidgety;

TEST(OptionsValidatorMessage, ValidatorMessageType) {
    ValidatorMessage valid(ValidatorMessageType::Valid);
    ValidatorMessage problematic(ValidatorMessageType::Problematic);
    ValidatorMessage invalid(ValidatorMessageType::Invalid);
    ValidatorMessage unexpected(ValidatorMessageType::Unexpected);

    EXPECT_EQ(valid.getMessageType(), ValidatorMessageType::Valid);
    EXPECT_EQ(problematic.getMessageType(), ValidatorMessageType::Problematic);
    EXPECT_EQ(invalid.getMessageType(), ValidatorMessageType::Invalid);
    EXPECT_EQ(unexpected.getMessageType(), ValidatorMessageType::Unexpected);
}
