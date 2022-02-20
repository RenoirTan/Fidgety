#include <fidgety/exception.hpp>
#include <gtest/gtest.h>

using namespace Fidgety;

TEST(ExceptionExceptions, GetterFunctions) {
    Exception exception(1, "information", "supporting info");
    EXPECT_EQ(exception.getCode(), 1);
    EXPECT_EQ(exception.getInformation(), "information");
    EXPECT_EQ(exception.getSupportingInformation(), "supporting info");
}

TEST(ExceptionExceptions, StringFormat) {
    Exception exception(1, "information", "detail 1\ndetail 2\ndetail 3");
    EXPECT_EQ(exception.getShortDescription(), "");
    EXPECT_EQ(exception.getGenericDescription(), ": information");
    EXPECT_EQ(
        exception.getLongDescription(),
        ": information\nDetails:\n\tdetail 1\n\tdetail 2\n\tdetail 3"
    );
}
