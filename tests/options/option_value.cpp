#include <string>
#include <fidgety/options.hpp>
#include <gtest/gtest.h>
#include "spdlog/spdlog.h"
#include "dummies.hpp"

using namespace Fidgety;

TEST(OptionsOptionValue, FromString) {
    OptionValue value("something", OptionValueType::RAW_VALUE);
    ASSERT_EQ(value.getValueType(), OptionValueType::RAW_VALUE);
    ASSERT_EQ(value.getValue().getRawValue(), "something");
}

TEST(OptionsOptionValue, FromArrayLike) {
    spdlog::set_level(spdlog::level::trace);

    OptionValue value(makeNestedOptionList(3), OptionValueType::NESTED_LIST);
    ASSERT_EQ(value.getValueType(), OptionValueType::NESTED_LIST);
    const NestedOptionList &nol = value.getValue().getNestedList();
    ASSERT_EQ(nol.size(), 3);
}

TEST(OptionsOptionValue, ChangeValueString) {
    OptionValue value("original", OptionValueType::RAW_VALUE);
    ASSERT_EQ(value.getValueType(), OptionValueType::RAW_VALUE);
    ASSERT_EQ(value.getValue().getRawValue(), "original");

    value.setValue("new");
    ASSERT_EQ(value.getValue().getRawValue(), "new");
}

TEST(OptionsOptionValue, ChangeValueList) {
    OptionValue value(makeNestedOptionList(3), OptionValueType::NESTED_LIST);
    {
        ASSERT_EQ(value.getValueType(), OptionValueType::NESTED_LIST);
        const NestedOptionList &nol = value.getValue().getNestedList();
        ASSERT_EQ(nol.size(), 3);
    }

    value.setValue(makeNestedOptionList(2));
    {
        ASSERT_EQ(value.getValueType(), OptionValueType::NESTED_LIST);
        const NestedOptionList &nol = value.getValue().getNestedList();
        ASSERT_EQ(nol.size(), 2);
    }
}

TEST(OptionsOptionValue, ChangeValueInvalidType) {
    OptionValue value("string", OptionValueType::RAW_VALUE);
    ASSERT_EQ(value.getValueType(), OptionValueType::RAW_VALUE);
    ASSERT_EQ(value.getValue().getRawValue(), "string");

    try {
        value.setValue(makeNestedOptionList(3));
    } catch (const OptionException &oe) {
        EXPECT_EQ(oe.getCode(), (int32_t) OptionStatus::InvalidValueType);
    }
}

TEST(OptionsOptionValue, ChangeValueAmbiguous) {
    OptionValue value("string", OptionValueType::RAW_VALUE | OptionValueType::NESTED_LIST);
    ASSERT_EQ(value.getValueType(), OptionValueType::RAW_VALUE);
    ASSERT_EQ(value.getValue().getRawValue(), "string");

    value.setValue(makeNestedOptionList(3));
    const NestedOptionList &nol = value.getValue().getNestedList();
    EXPECT_EQ(nol.size(), 3);
}

TEST(OptionsOptionValue, ResetString) {
    OptionValue value("original", OptionValueType::RAW_VALUE);
    ASSERT_EQ(value.getValueType(), OptionValueType::RAW_VALUE);
    ASSERT_EQ(value.getValue().getRawValue(), "original");

    value.setValue("new");
    value.resetValue();
    ASSERT_EQ(value.getValue().getRawValue(), "original");
}

TEST(OptionsOptionValue, ResetList) {
    OptionValue value(makeNestedOptionList(3), OptionValueType::NESTED_LIST);
    {
        ASSERT_EQ(value.getValueType(), OptionValueType::NESTED_LIST);
        const NestedOptionList &nol = value.getValue().getNestedList();
        ASSERT_EQ(nol.size(), 3);
    }

    value.setValue(makeNestedOptionList(2));
    value.resetValue();
    {
        ASSERT_EQ(value.getValueType(), OptionValueType::NESTED_LIST);
        const NestedOptionList &nol = value.getValue().getNestedList();
        ASSERT_EQ(nol.size(), 3);
    }
}

TEST(OptionsOptionValue, SetAcceptedValueTypes) {
    OptionValue value("stuff", OptionValueType::RAW_VALUE);
    ASSERT_EQ(value.getValueType(), OptionValueType::RAW_VALUE);
    ASSERT_EQ(value.getValue().getRawValue(), "stuff");

    value.setAcceptedValueTypes(OptionValueType::RAW_VALUE | OptionValueType::NESTED_LIST);
    value.setValue(makeNestedOptionList(3));
    {
        ASSERT_EQ(value.getValueType(), OptionValueType::NESTED_LIST);
        const NestedOptionList &nol = value.getValue().getNestedList();
        ASSERT_EQ(nol.size(), 3);
    }
}

TEST(OptionsOptionValue, SetInvalidAcceptedValueTypes) {
    OptionValue value("stuff", OptionValueType::RAW_VALUE);
    ASSERT_EQ(value.getValueType(), OptionValueType::RAW_VALUE);
    ASSERT_EQ(value.getValue().getRawValue(), "stuff");

    try {
        value.setAcceptedValueTypes(OptionValueType::NESTED_LIST);
    } catch (const OptionException &oe) {
        EXPECT_EQ(oe.getCode(), (int32_t) OptionStatus::InvalidValueType);
    }
}
