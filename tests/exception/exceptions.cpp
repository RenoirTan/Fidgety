/**
 * @file tests/exception/exceptions.cpp
 * @author RenoirTan
 * @brief Test suite for making sure that Fidgety::Exception outputs the
 * correct message.
 * @version 0.1
 * @date 2022-03-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <cstring>
#include <fidgety/_tests.hpp>
#include <fidgety/exception.hpp>
#include <spdlog/spdlog.h>
#include <gtest/gtest.h>

using namespace Fidgety;

TEST(ExceptionExceptions, GetterFunctions) {
    _FIDGETY_INIT_TEST();
    Exception exception(1, "information", "supporting info");
    EXPECT_EQ(exception.getCode(), 1);
    EXPECT_EQ(exception.getInformation(), "information");
    EXPECT_EQ(exception.getSupportingInformation(), "supporting info");
}

TEST(ExceptionExceptions, StringFormat) {
    _FIDGETY_INIT_TEST();
#define EXPECTED_ANSWER ": information\nDetails:\n\tdetail 1\n\tdetail 2\n\tdetail 3"
    Exception exception(1, "information", "detail 1\ndetail 2\ndetail 3");
    EXPECT_EQ(exception.getShortDescription(), "");
    EXPECT_EQ(exception.getGenericDescription(), ": information");
    EXPECT_EQ(exception.getLongDescription(), EXPECTED_ANSWER);
#ifdef FIDGETY_EXCEPTION_WHAT_USE_DESCRIPTION
    EXPECT_EQ(strcmp(exception.what(), EXPECTED_ANSWER), 0);
#else
    EXPECT_EQ(strcmp(exception.what(), "A Fidgety::Exception occurred."), 0);
#endif
#undef EXPECTED_ANSWER
}

TEST(ExceptionExceptions, Exit) {
    _FIDGETY_INIT_TEST();
    Exception exception(1, "information", "supporting info");
    EXPECT_EXIT(exception.exit(true, true), testing::ExitedWithCode(1), "");
}
