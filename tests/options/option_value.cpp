/**
 * @file tests/options/option_value.cpp
 * @author RenoirTan
 * @brief Test suite to make sure that Fidgety::OptionValue can properly handle
 * values and default values.
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

TEST(OptionsOptionValue, FromString) {
    _FIDGETY_INIT_TEST();
    OptionValue value("something", OptionValueType::RAW_VALUE);
    ASSERT_EQ(value.getValueType(), OptionValueType::RAW_VALUE);
    ASSERT_EQ(value.getValue().getRawValue(), "something");
}

TEST(OptionsOptionValue, FromArrayLike) {
    _FIDGETY_INIT_TEST();

    std::pair<OptionsMap, NestedOptionNameList> res = makeNestedOptionList(3);
    OptionsMap omap = res.first;
    NestedOptionNameList nonl = res.second;
    OptionValue value(std::move(nonl), OptionValueType::NESTED_LIST);
    ASSERT_EQ(value.getValueType(), OptionValueType::NESTED_LIST);
    const NestedOptionNameList &nol = value.getValue().getNestedList();
    ASSERT_EQ(nol.size(), 3);
}

TEST(OptionsOptionValue, ChangeValueString) {
    _FIDGETY_INIT_TEST();

    OptionValue value("original", OptionValueType::RAW_VALUE);
    ASSERT_EQ(value.getValueType(), OptionValueType::RAW_VALUE);
    ASSERT_EQ(value.getValue().getRawValue(), "original");

    value.setValue("new");
    ASSERT_EQ(value.getValue().getRawValue(), "new");
}

TEST(OptionsOptionValue, ChangeValueList) {
    _FIDGETY_INIT_TEST();

    std::pair<OptionsMap, NestedOptionNameList> res = makeNestedOptionList(3);
    OptionsMap omap = res.first;
    NestedOptionNameList nonl = res.second;
    OptionValue value(std::move(nonl), OptionValueType::NESTED_LIST);
    {
        ASSERT_EQ(value.getValueType(), OptionValueType::NESTED_LIST);
        const NestedOptionNameList &nol = value.getValue().getNestedList();
        ASSERT_EQ(nol.size(), 3);
    }

    res = makeNestedOptionList(2);
    nonl = res.second;
    value.setValue(std::move(nonl));
    {
        ASSERT_EQ(value.getValueType(), OptionValueType::NESTED_LIST);
        const NestedOptionNameList &nol = value.getValue().getNestedList();
        ASSERT_EQ(nol.size(), 2);
    }
}

TEST(OptionsOptionValue, ChangeValueInvalidType) {
    _FIDGETY_INIT_TEST();

    OptionValue value("string", OptionValueType::RAW_VALUE);
    ASSERT_EQ(value.getValueType(), OptionValueType::RAW_VALUE);
    ASSERT_EQ(value.getValue().getRawValue(), "string");

    try {
        std::pair<OptionsMap, NestedOptionNameList> res = makeNestedOptionList(3);
        NestedOptionNameList nonl = res.second;
        value.setValue(std::move(nonl));
    } catch (const OptionException &oe) {
        EXPECT_EQ(oe.getCode(), (int32_t) OptionStatus::InvalidValueType);
    }
}

TEST(OptionsOptionValue, ChangeValueAmbiguous) {
    _FIDGETY_INIT_TEST();

    OptionValue value("string", OptionValueType::RAW_VALUE | OptionValueType::NESTED_LIST);
    ASSERT_EQ(value.getValueType(), OptionValueType::RAW_VALUE);
    ASSERT_EQ(value.getValue().getRawValue(), "string");

    std::pair<OptionsMap, NestedOptionNameList> res = makeNestedOptionList(3);
    OptionsMap omap = res.first;
    NestedOptionNameList nonl = res.second;
    value.setValue(std::move(nonl));
    const NestedOptionNameList &nol = value.getValue().getNestedList();
    EXPECT_EQ(nol.size(), 3);
}

TEST(OptionsOptionValue, ResetString) {
    _FIDGETY_INIT_TEST();

    OptionValue value("original", OptionValueType::RAW_VALUE);
    ASSERT_EQ(value.getValueType(), OptionValueType::RAW_VALUE);
    ASSERT_EQ(value.getValue().getRawValue(), "original");

    value.setValue("new");
    value.resetValue();
    ASSERT_EQ(value.getValue().getRawValue(), "original");
}

TEST(OptionsOptionValue, ResetList) {
    _FIDGETY_INIT_TEST();

    std::pair<OptionsMap, NestedOptionNameList> res = makeNestedOptionList(3);
    OptionsMap omap = res.first;
    NestedOptionNameList nonl = res.second;
    OptionValue value(std::move(nonl), OptionValueType::NESTED_LIST);
    {
        ASSERT_EQ(value.getValueType(), OptionValueType::NESTED_LIST);
        const NestedOptionNameList &nol = value.getValue().getNestedList();
        ASSERT_EQ(nol.size(), 3);
    }

    res = makeNestedOptionList(2);
    nonl = res.second;
    value.setValue(std::move(nonl));
    value.resetValue();
    {
        ASSERT_EQ(value.getValueType(), OptionValueType::NESTED_LIST);
        const NestedOptionNameList &nol = value.getValue().getNestedList();
        ASSERT_EQ(nol.size(), 3);
    }
}

TEST(OptionsOptionValue, SetAcceptedValueTypes) {
    _FIDGETY_INIT_TEST();

    OptionValue value("stuff", OptionValueType::RAW_VALUE);
    ASSERT_EQ(value.getValueType(), OptionValueType::RAW_VALUE);
    ASSERT_EQ(value.getValue().getRawValue(), "stuff");

    value.setAcceptedValueTypes(OptionValueType::RAW_VALUE | OptionValueType::NESTED_LIST);
    std::pair<OptionsMap, NestedOptionNameList> res = makeNestedOptionList(3);
    OptionsMap omap = res.first;
    NestedOptionNameList nonl = res.second;
    value.setValue(std::move(nonl));
    {
        ASSERT_EQ(value.getValueType(), OptionValueType::NESTED_LIST);
        const NestedOptionNameList &nol = value.getValue().getNestedList();
        ASSERT_EQ(nol.size(), 3);
    }
}

TEST(OptionsOptionValue, SetInvalidAcceptedValueTypes) {
    _FIDGETY_INIT_TEST();

    OptionValue value("stuff", OptionValueType::RAW_VALUE);
    ASSERT_EQ(value.getValueType(), OptionValueType::RAW_VALUE);
    ASSERT_EQ(value.getValue().getRawValue(), "stuff");

    try {
        value.setAcceptedValueTypes(OptionValueType::NESTED_LIST);
    } catch (const OptionException &oe) {
        EXPECT_EQ(oe.getCode(), (int32_t) OptionStatus::InvalidValueType);
    }
}
