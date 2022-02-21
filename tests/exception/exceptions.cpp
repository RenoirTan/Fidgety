#include <cstring>
#include <fidgety/exception.hpp>
#include "spdlog/spdlog.h"
#include <gtest/gtest.h>

using namespace Fidgety;

TEST(ExceptionExceptions, GetterFunctions) {
    Exception exception(1, "information", "supporting info");
    EXPECT_EQ(exception.getCode(), 1);
    EXPECT_EQ(exception.getInformation(), "information");
    EXPECT_EQ(exception.getSupportingInformation(), "supporting info");
}

TEST(ExceptionExceptions, StringFormat) {
#define EXPECTED_ANSWER ": information\nDetails:\n\tdetail 1\n\tdetail 2\n\tdetail 3"
    Exception exception(1, "information", "detail 1\ndetail 2\ndetail 3");
    EXPECT_EQ(exception.getShortDescription(), "");
    EXPECT_EQ(exception.getGenericDescription(), ": information");
    EXPECT_EQ(exception.getLongDescription(), EXPECTED_ANSWER);
#undef EXPECTED_ANSWER
    EXPECT_EQ(strcmp(exception.what(), "A Fidgety::Exception occurred."), 0);
}

TEST(ExceptionExceptions, Exit) {
    Exception exception(1, "information", "supporting info");
    EXPECT_EXIT(exception.exit(true, true), testing::ExitedWithCode(1), "");
}
