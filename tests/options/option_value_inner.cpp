/**
 * @file tests/options/option_value_inner.cpp
 * @author RenoirTan
 * @brief Test suite to ensure Fidgety::OptionValueInner manages the raw union
 * data properly.
 * @version 0.1
 * @date 2022-03-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <string>
#include <fidgety/options.hpp>
#include <gtest/gtest.h>
#include "spdlog/spdlog.h"

#include "dummies.hpp"

using namespace Fidgety;

TEST(OptionsOptionValueInner, FromString) {
    _FIDGETY_INIT_TEST();

    {
        spdlog::debug("creating fromCharArray");
        OptionValueInner fromCharArray("thing");
        spdlog::debug("fromCharArray created");
        spdlog::debug("checking fromCharArray's valueType");
        ASSERT_EQ(fromCharArray.valueType, OptionValueType::RAW_VALUE);
        spdlog::debug("checking fromCharArray's raw value");
        EXPECT_EQ(fromCharArray.getRawValue(), "thing");
        spdlog::debug("fromCharArray successful");
        spdlog::debug("deleting fromCharArray");
        fromCharArray.~OptionValueInner();
    }

    {
        spdlog::debug("creating fromString");
        OptionValueInner fromString(std::string("input"));
        spdlog::debug("fromString created");
        spdlog::debug("checking fromString's valueType");
        ASSERT_EQ(fromString.valueType, OptionValueType::RAW_VALUE);
        spdlog::debug("checking fromString's raw value");
        EXPECT_EQ(fromString.getRawValue(), "input");
        spdlog::debug("fromString successful");
        spdlog::debug("deleting fromString");
    }
}

TEST(OptionsOptionValueInner, FromArrayLike) {
    _FIDGETY_INIT_TEST();

    spdlog::debug("creating dummy nested option list for fromArrayLike");
    Fidgety::NestedOptionList nol = makeNestedOptionList(10);
    spdlog::debug("creating fromVector");
    OptionValueInner fromVector(std::move(nol));
    spdlog::debug("fromVector created");
    spdlog::debug("checking fromVector's valueType");
    ASSERT_EQ(fromVector.valueType, OptionValueType::NESTED_LIST);
    spdlog::debug("getting reference to the nested list stored inside fromVector");
    const Fidgety::NestedOptionList &stored = fromVector.getNestedList();
    spdlog::debug("checking the length of the nested list");
    ASSERT_EQ(stored.size(), 10);
    spdlog::debug("making sure the identifier at index 5 is correct");
    EXPECT_EQ(stored.at(5)->getIdentifier(), "Option 5");
}
