/**
 * @file tests/options/validator.cpp
 * @author RenoirTan
 * @brief Test suite for Fidgety::Validator.
 * @version 0.1
 * @date 2022-03-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <string>
#include <fidgety/options.hpp>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>
#include "dummies.hpp"

using namespace Fidgety;

TEST(OptionsValidator, Validator) {
    _FIDGETY_INIT_TEST();
    std::unique_ptr<Validator> validator(new Validator());
    OptionEditor editor(OptionEditorType::Blanked, std::map<std::string, std::string>());
    OptionValue value("value", OptionValueType::RAW_VALUE);
    Option option("dummy", std::move(editor), std::move(validator), std::move(value));

    ValidatorContextInner vci = makeValidatorContextInner(10);
    ValidatorContext context(std::move(vci));
    
    ValidatorMessage message = option.validate(context);
    EXPECT_EQ(message.fullMessage(), "Valid: Ok");
}
