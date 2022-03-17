/**
 * @file tests/dylib/dylib_hello.cpp
 * @author RenoirTan
 * @brief Test suites for dylib using a Hello class.
 * @version 0.1
 * @date 2022-03-16
 * 
 * @copyright Copyright (c) 2022
 */

#include <fidgety/_tests.hpp>
#include <fidgety/dylib.hpp>
#include <gtest/gtest.h>
#include "spdlog/spdlog.h"
#include "dylib_hello.hpp"

using namespace Fidgety;

TEST(DylibHello, FullRun) {
    _FIDGETY_INIT_TEST();
    DyclassLoader<Hello> loader("./libdylib_hello_lib.so");
    ASSERT_EQ(loader.openLibrary(), DylibStatus::Ok);

    {
        DyclassBox<Hello> hello = loader.getInstance();
        EXPECT_EQ(hello->greet(), "Hello, my name is Renoir");
    }

    ASSERT_EQ(loader.closeLibrary(), DylibStatus::Ok);
}
