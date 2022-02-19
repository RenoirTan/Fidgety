if(FIDGETY_GTEST_FROM_SOURCE)
    message(STATUS "downloading googletest's source code and building it from source")
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL ${FIDGETY_GOOGLETEST_RELEASE_URL}
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    include(GoogleTest)
    set(GTEST gtest)
    set(GTEST_MAIN gtest_main)
    set(GMOCK gmock)
    set(GMOCK_MAIN gmock_main)
else()
    message(STATUS "searching for googletest using CMake's find_package")
    find_package(gtest CONFIG REQUIRED)
    include(GoogleTest)
    set(GTEST GTest::gtest)
    set(GTEST_MAIN GTest::gtest)
    set(GMOCK GTest::gmock)
    set(GMOCK_MAIN GTest::gmock)
endif()