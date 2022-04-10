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
#include <spdlog/spdlog.h>

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
    std::pair<OptionsMap, NestedOptionNameList> res = makeNestedOptionList(10);
    OptionsMap omap = res.first; NestedOptionNameList nonl = res.second;
    spdlog::debug("creating fromVector");
    OptionValueInner fromVector(std::move(nonl));
    spdlog::debug("fromVector created");
    spdlog::debug("checking fromVector's valueType");
    ASSERT_EQ(fromVector.valueType, OptionValueType::NESTED_LIST);
    spdlog::debug("getting reference to the nested list stored inside fromVector");
    const Fidgety::NestedOptionNameList &stored = fromVector.getNestedList();
    spdlog::debug("checking the length of the nested list");
    ASSERT_EQ(stored.size(), 10);
    spdlog::debug("making sure the identifier at index 5 is correct");
    EXPECT_EQ(stored.at(5), "Option 5");
    {
        std::shared_ptr<Option> option = omap[stored.at(5)];
        EXPECT_EQ(option->getValueType(), OptionValueType::RAW_VALUE);
        EXPECT_EQ(option->getRawValue(), "");
    }
}
