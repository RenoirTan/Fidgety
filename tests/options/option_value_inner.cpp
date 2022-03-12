#include <string>
#include <fidgety/options.hpp>
#include <gtest/gtest.h>
#include "spdlog/spdlog.h"

#include "dummies.hpp"

using namespace Fidgety;

TEST(OptionsOptionValueInner, FromString) {
    OptionValueInner fromCharArray("thing");
    ASSERT_EQ(fromCharArray.valueType, OptionValueType::RAW_VALUE);
    EXPECT_EQ(fromCharArray.getRawValue(), "thing");

    OptionValueInner fromString(std::string("input"));
    ASSERT_EQ(fromString.valueType, OptionValueType::RAW_VALUE);
    EXPECT_EQ(fromString.getRawValue(), "input");
}

TEST(OptionsOptionValueInner, FromArrayLike) {
    Fidgety::NestedOptionList nol = makeNestedOptionList(10);
    OptionValueInner fromVector(std::move(nol));
    ASSERT_EQ(fromVector.valueType, OptionValueType::NESTED_LIST);
    const Fidgety::NestedOptionList &stored = fromVector.getNestedList();
    ASSERT_EQ(stored.size(), 10);
    EXPECT_EQ(stored.at(5)->getIdentifier(), "Option 5");
}
