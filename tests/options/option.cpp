/**
 * @file tests/options/option.cpp
 * @author RenoirTan
 * @brief Make sure that Fidgety::Option works.
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

TEST(OptionsOption, SanityCheck) {
    _FIDGETY_INIT_TEST();
    std::unique_ptr<Validator> validator(new Validator());
    OptionEditor editor(OptionEditorType::TextEntry, std::map<std::string, std::string>());
    OptionValue value("value", OptionValueType::RAW_VALUE);
    Option option("option", std::move(editor), std::move(validator), std::move(value));

    EXPECT_EQ(option.getIdentifier(), "option");
    ASSERT_EQ(option.getValueType(), OptionValueType::RAW_VALUE);
    EXPECT_EQ(option.getValue().getRawValue(), "value");
    ASSERT_EQ(option.getDefaultValueType(), OptionValueType::RAW_VALUE);
    EXPECT_EQ(option.getDefaultValue().getRawValue(), "value");

    try {
        option.getNestedList();
    } catch (const OptionException &oe) {
        EXPECT_EQ(oe.getCode(), (int32_t) OptionStatus::InvalidValueType);
    }

    try {
        option.getDefaultNestedList();
    } catch (const OptionException &oe) {
        EXPECT_EQ(oe.getCode(), (int32_t) OptionStatus::InvalidValueType);
    }

    ValidatorContextInner vci = makeValidatorContextInner(1);
    ValidatorContext context(std::move(vci));
    ValidatorMessage message = option.validate(context);
    EXPECT_EQ(message.fullMessage(), "Valid: Ok");
}

TEST(OptionsOption, SetValues) {
    _FIDGETY_INIT_TEST();
    std::unique_ptr<Validator> validator(new Validator());
    OptionEditor editor(OptionEditorType::TextEntry, std::map<std::string, std::string>());
    OptionValue value("original", OptionValueType::RAW_VALUE);
    Option option("option", std::move(editor), std::move(validator), std::move(value));

    EXPECT_EQ(option.getIdentifier(), "option");
    ASSERT_EQ(option.getValueType(), OptionValueType::RAW_VALUE);
    EXPECT_EQ(option.getValue().getRawValue(), "original");
    ASSERT_EQ(option.getDefaultValueType(), OptionValueType::RAW_VALUE);
    EXPECT_EQ(option.getDefaultValue().getRawValue(), "original");

    option.setValue("new");
    ASSERT_EQ(option.getValueType(), OptionValueType::RAW_VALUE);
    EXPECT_EQ(option.getValue().getRawValue(), "new");

    option.resetValue();
    ASSERT_EQ(option.getValueType(), OptionValueType::RAW_VALUE);
    EXPECT_EQ(option.getValue().getRawValue(), "original");

    option.setAcceptedValueTypes(OptionValueType::RAW_VALUE | OptionValueType::NESTED_LIST);
    option.setValue(makeNestedOptionList(3));
    ASSERT_EQ(option.getValueType(), OptionValueType::NESTED_LIST);
    {
        const NestedOptionList &nol = option.getNestedList();
        EXPECT_EQ(nol.size(), 3);
    }

    option.resetValue();
    ASSERT_EQ(option.getValueType(), OptionValueType::RAW_VALUE);
    EXPECT_EQ(option.getValue().getRawValue(), "original");
}
