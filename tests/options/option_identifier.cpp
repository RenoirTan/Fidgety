/**
 * @file tests/options/option_identifier.cpp
 * @author RenoirTan
 * @brief Tests for OptionIdentifier.
 * @version 0.1
 * @date 2022-04-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <fidgety/_tests.hpp>
#include <fidgety/options.hpp>
#include <gtest/gtest.h>

using namespace Fidgety;

#define _VALID_ID_STR "appearance.theme.icon.priority.4"

static inline OptionIdentifier _createIdentifier(void) {
    return OptionIdentifier(_VALID_ID_STR);
}

#define _PARTS {"appearance", "theme", "icon", "priority", "4"}

TEST(OptionsOptionIdentifier, Identifier) {
    _FIDGETY_INIT_TEST();
    OptionIdentifier identifier = _createIdentifier();
    EXPECT_TRUE(identifier.isValid());
    EXPECT_EQ((std::string) identifier, _VALID_ID_STR);
    EXPECT_EQ(identifier.depth(), 5);
    EXPECT_EQ(identifier.split(), std::vector<OptionName>(_PARTS));
}

TEST(OptionsOptionIdentifier, InvalidIdentifier) {
    _FIDGETY_INIT_TEST();
#define _CASE(id) { \
    OptionIdentifier identifier(id); \
    EXPECT_FALSE(identifier.isValid()); \
}

    _CASE("");
    _CASE("a..b");
    _CASE("a.1.2c");

#undef _CASE
}

TEST(OptionsOptionIdentifier, Iterator) {
    _FIDGETY_INIT_TEST();
    OptionIdentifier identifier = _createIdentifier();
    const char *PARTS[] = _PARTS;
    size_t index = 0;
    for (const auto &it : identifier) {
        spdlog::info("[OptionsOptionIdentifier::Iterator] index: {0}", index);
        EXPECT_EQ(it, PARTS[index]);
        ++index;
    }
    EXPECT_EQ(identifier.at(5), identifier.end());
    EXPECT_EQ(identifier.at(1)+2, identifier.at(3));
    EXPECT_EQ(identifier.at(4)-4, identifier.at(0));
    EXPECT_EQ(identifier.at(2)-3, identifier.end());
    EXPECT_EQ(identifier.at(0), identifier.begin());
    EXPECT_EQ(identifier.end(), identifier.end());
    EXPECT_EQ(OptionIdentifier("somewhere.hi"), OptionIdentifier("somewhere.hi"));
    EXPECT_EQ(OptionIdentifier("somewhere.hi"), "somewhere.hi");
    EXPECT_EQ("somewhere.hi", OptionIdentifier("somewhere.hi"));
    EXPECT_NE(OptionIdentifier("somewhere.hi"), OptionIdentifier("somewhere.bye"));
    EXPECT_NE(OptionIdentifier("somewhere.hi"), "somewhere.bye");
    EXPECT_NE("somewhere.hi", OptionIdentifier("somewhere.bye"));
}