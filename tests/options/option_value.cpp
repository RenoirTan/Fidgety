#include <string>
#include <fidgety/options.hpp>
#include <gtest/gtest.h>
#include "spdlog/spdlog.h"
#include "dummies.hpp"

using namespace Fidgety;

TEST(OptionsOptionValue, FromString) {
    OptionValue value("something");
    ASSERT_EQ(value.getValueType(), OptionValueType::RAW_VALUE);
    ASSERT_EQ(value.getValue().getRawValue(), "something");
}

TEST(OptionsOptionValue, FromArrayLike) {
    spdlog::set_level(spdlog::level::trace);

    OptionValue value(OptionValueInner(makeNestedOptionList(10)));
    ASSERT_EQ(value.getValueType(), OptionValueType::NESTED_LIST);
    const NestedOptionList &nol = value.getValue().getNestedList();
    ASSERT_EQ(nol.size(), 10);
}

TEST(OptionsOptionValue, ChangeValueString) {
    OptionValue value("original");
    ASSERT_EQ(value.getValueType(), OptionValueType::RAW_VALUE);
    ASSERT_EQ(value.getValue().getRawValue(), "original");

    value.setValue("new");
    ASSERT_EQ(value.getValue().getRawValue(), "new");
}

TEST(OptionsOptionValue, ChangeValueList) {
    OptionValue value(makeNestedOptionList(10));
    {
        ASSERT_EQ(value.getValueType(), OptionValueType::NESTED_LIST);
        const NestedOptionList &nol = value.getValue().getNestedList();
        ASSERT_EQ(nol.size(), 10);
    }

    value.setValue(makeNestedOptionList(5));
    {
        ASSERT_EQ(value.getValueType(), OptionValueType::NESTED_LIST);
        const NestedOptionList &nol = value.getValue().getNestedList();
        ASSERT_EQ(nol.size(), 5);
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
    OptionValue value("original");
    ASSERT_EQ(value.getValueType(), OptionValueType::RAW_VALUE);
    ASSERT_EQ(value.getValue().getRawValue(), "original");

    value.setValue("new");
    value.resetValue();
    ASSERT_EQ(value.getValue().getRawValue(), "original");
}

TEST(OptionsOptionValue, ResetList) {
    OptionValue value(makeNestedOptionList(10));
    {
        ASSERT_EQ(value.getValueType(), OptionValueType::NESTED_LIST);
        const NestedOptionList &nol = value.getValue().getNestedList();
        ASSERT_EQ(nol.size(), 10);
    }

    value.setValue(makeNestedOptionList(5));
    value.resetValue();
    {
        ASSERT_EQ(value.getValueType(), OptionValueType::NESTED_LIST);
        const NestedOptionList &nol = value.getValue().getNestedList();
        ASSERT_EQ(nol.size(), 10);
    }
}

TEST(OptionsOptionValue, SetAcceptedValueTypes) {
    OptionValue value("stuff");
    ASSERT_EQ(value.getValueType(), OptionValueType::RAW_VALUE);
    ASSERT_EQ(value.getValue().getRawValue(), "stuff");

    value.setAcceptedValueTypes(OptionValueType::RAW_VALUE | OptionValueType::NESTED_LIST);
    value.setValue(makeNestedOptionList(10));
    {
        ASSERT_EQ(value.getValueType(), OptionValueType::NESTED_LIST);
        const NestedOptionList &nol = value.getValue().getNestedList();
        ASSERT_EQ(nol.size(), 10);
    }
}

TEST(OptionsOptionValue, SetInvalidAcceptedValueTypes) {
    OptionValue value("stuff");
    ASSERT_EQ(value.getValueType(), OptionValueType::RAW_VALUE);
    ASSERT_EQ(value.getValue().getRawValue(), "stuff");

    try {
        value.setAcceptedValueTypes(OptionValueType::NESTED_LIST);
    } catch (const OptionException &oe) {
        EXPECT_EQ(oe.getCode(), (int32_t) OptionStatus::InvalidValueType);
    }
}
