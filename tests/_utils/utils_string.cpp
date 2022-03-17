/**
 * @file tests/utils/utils_string.cpp
 * @author RenoirTan
 * @brief Tests for string utilities.
 * @version 0.1
 * @date 2022-03-17
 * 
 * @copyright Copyright (c) 2022
 */

#include <boost/algorithm/string/case_conv.hpp>
#include <fidgety/_tests.hpp>
#include <fidgety/_utils.hpp>
#include <gtest/gtest.h>

using namespace Fidgety;

TEST(UtilsString, TrimLeft) {
    _FIDGETY_INIT_TEST();
    std::string s = " \t something ";
    ltrim(s);
    ASSERT_EQ(s, "something ");
}

TEST(UtilsString, TrimRight) {
    _FIDGETY_INIT_TEST();
    std::string s = " something\t\t    ";
    rtrim(s);
    ASSERT_EQ(s, " something");
}

TEST(UtilsString, Trim) {
    _FIDGETY_INIT_TEST();
    std::string s = "\t\t   something \t ";
    trim(s);
    ASSERT_EQ(s, "something");
}

TEST(UtilsString, TrimPristine) {
    _FIDGETY_INIT_TEST();
    std::string s = "no flanking whitespace";
    trim(s);
    ASSERT_EQ(s, "no flanking whitespace");
}

TEST(UtilsString, TruncateAfter) {
    _FIDGETY_INIT_TEST();
    std::string s = "line with comment #comment";
    truncateAfter(s, "#");
    ASSERT_EQ(s, "line with comment ");
}

TEST(UtilsString, TruncateAfterInsensitive) {
    _FIDGETY_INIT_TEST();
    std::string s = "line with guard SeNtInEl byebye";
    truncateAfter(s, "sentinel", true);
    ASSERT_EQ(s, "line with guard ");
}

TEST(UtilsString, IsEffectivelyEmpty) {
    _FIDGETY_INIT_TEST();
    EXPECT_TRUE(isEffectivelyEmpty("\t    \t\n \v \f   \r"));
    EXPECT_FALSE(isEffectivelyEmpty("  \t\t  hi \b"));
    EXPECT_FALSE(isEffectivelyEmpty("yes"));
}

TEST(UtilsString, Sed) {
    _FIDGETY_INIT_TEST();
    class ToUpperEditor : public StringEditor {
        void mutateString(std::string &s) {
            boost::to_upper(s);
        }

        std::string convertString(const std::string &s) {
            return boost::to_upper_copy(s);
        }
    };

    ToUpperEditor tue;
    std::string s = "abc\ndef\nghi";
    std::string f = sed(s, &tue);
    ASSERT_EQ(f, "ABC\nDEF\nGHI");
}

TEST(UtilsString, TabIndentSed) {
    _FIDGETY_INIT_TEST();
    ASSERT_EQ(tabIndentSed("1\n2\n3"), "\t1\n\t2\n\t3");
}

TEST(UtilsString, SpaceIndentSed) {
    _FIDGETY_INIT_TEST();
    ASSERT_EQ(spaceIndentSed("1\n2\n3"), "    1\n    2\n    3");
}
