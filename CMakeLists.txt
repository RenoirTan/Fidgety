cmake_minimum_required(VERSION 3.14)
project(Fidgety)
list(APPEND CMAKE_MODULE_PATH cmake/)

include(GNUInstallDirs)
include(add_my_library)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 90)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)
find_package(spdlog REQUIRED)
find_package(nlohmann_json REQUIRED)

set(
    FIDGETY_EXPORT_CMAKE_DIR
    "${CMAKE_INSTALL_LIBDIR}/cmake/fidgety"
    CACHE STRING "The directory to dump all the *.cmake files used to find fidgety."
)
set(
    FIDGETY_EXPORT_LIB_DIR
    "${CMAKE_INSTALL_LIBDIR}/fidgety"
    CACHE STRING "The directory to dump all of fidgety's library files."
)
set(
    FIDGETY_GOOGLETEST_RELEASE_URL
    "https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip"
    CACHE STRING "The URL of the release of googletest to use"
)

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
    DESTINATION ${FIDGETY_EXPORT_CMAKE_DIR}
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
    include(googletest_include)
    # test directory
    message(STATUS "adding 'tests' subdirectory")
    add_subdirectory(tests)
endif()
