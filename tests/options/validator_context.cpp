#include <string>
#include <fidgety/options.hpp>
#include <gtest/gtest.h>
#include "spdlog/spdlog.h"
#include "dummies.hpp"

using namespace Fidgety;

TEST(OptionsValidatorContext, FromEmpty) {
    ValidatorContext context;
    EXPECT_FALSE(context.optionExists("thing"));
    try {
        const Option &option = context.getOption("thing");
    } catch (const OptionException &oe) {
        EXPECT_EQ(oe.getCode(), (int32_t) OptionStatus::NotFound);
    }
}

TEST(OptionsValidatorContext, FromDummies) {
    ValidatorContextInner vci = makeValidatorContextInner(10);
    ValidatorContext context(std::move(vci));
    ASSERT_TRUE(context.optionExists("Option 5"));
    const Option &option5 = context.getOption("Option 5");
    ASSERT_EQ(option5.getValueType(), OptionValueType::RAW_VALUE);
    ASSERT_EQ(option5.getRawValue(), "");
    try {
        const NestedOptionList &nol = option5.getNestedList();
    } catch (const OptionException &oe) {
        EXPECT_EQ(oe.getCode(), (int32_t) OptionStatus::InvalidValueType);
    }
}