cmake_minimum_required(VERSION 3.14)
project(Fidgety)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)
find_package(spdlog REQUIRED)
find_package(nlohmann_json REQUIRED)

include(GNUInstallDirs)

set(export_dest_dir "${CMAKE_INSTALL_LIBDIR}/cmake/spdlog")

#include_directories(include)
#file(GLOB_RECURSE fidgety_headers "include/*")
add_library(FidgetyHeaders INTERFACE)
add_library(Fidgety::FidgetyHeaders ALIAS FidgetyHeaders)
# include everything under the include/ folder
target_include_directories(
    FidgetyHeaders INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
install(DIRECTORY include/ DESTINATION include FILES_MATCHING PATTERN *.hpp)
install(TARGETS FidgetyHeaders EXPORT FidgetyHeaders DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(
    EXPORT FidgetyHeaders
    DESTINATION ${export_dest_dir}
    NAMESPACE Fidgety::
    FILE fidgety_headers_config.cmake
)

add_subdirectory(src)

set(FIDGETY_BUILD_TESTING ON CACHE BOOL "whether to build fidgety's tests")
set(
    FIDGETY_GTEST_FROM_SOURCE ON
    CACHE BOOL "whether to download googletest and build it from source (recommended for windows)"
)
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR FIDGETY_BUILD_TESTING) AND BUILD_TESTING)
    message(STATUS "enabling testing")

    # googletest stuff
    if(FIDGETY_GTEST_FROM_SOURCE)
        message(STATUS "downloading googletest's source code and building it from source")
        include(FetchContent)
        FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
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

    # test directory
    message(STATUS "adding 'tests' subdirectory")
    add_subdirectory(tests)
endif()
