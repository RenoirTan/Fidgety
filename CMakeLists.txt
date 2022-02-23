cmake_minimum_required(VERSION 3.14)
project(Fidgety)

include(GNUInstallDirs)
include(cmake/add_my_library.cmake)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 90)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(
    FIDGETY_EXPORT_CMAKE_DIR
    "${CMAKE_INSTALL_LIBDIR}/cmake/fidgety"
    CACHE STRING "The directory to dump all the *.cmake files used to find fidgety"
)
set(
    FIDGETY_EXPORT_LIB_DIR
    "${CMAKE_INSTALL_LIBDIR}/fidgety"
    CACHE STRING "The directory to dump all of fidgety's library files"
)
set(
    FIGETY_EXPORT_EXTLIB_DIR
    "${CMAKE_INSTALL_LIBDIR}/fidgety/extensions"
    CACHE STRING "The directory where fidgety's extension libraries are installed to"
)
set(
    FIDGETY_GOOGLETEST_RELEASE_URL
    "https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip"
    CACHE STRING "The URL of the release of googletest to use"
)
set(FIDGETY_QT_MAJOR 6 CACHE STRING "The major version number of Qt being used to build Fidgety")

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(FIDGETY_MASTER_PROJECT ON)
else()
    set(FIDGETY_MASTER_PROJECT OFF)
endif()

set(
    FIDGETY_BUILD_EXTENSIONS ${FIDGETY_MASTER_PROJECT}
    CACHE BOOL "Whether to build the extensions used by the fidgety editor"
)

set(FIDGETY_CONFIG_HEADER_IN "${PROJECT_SOURCE_DIR}/include/fidgety/config.h.in")
set(FIDGETY_CONFIG_HEADER_OUT "${PROJECT_SOURCE_DIR}/include/fidgety/config.h")

if(FIDGETY_QT_MAJOR STREQUAL 6)
    find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)
elseif(FIDGETY_QT_MAJOR STREQUAL 5)
    find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
else()
    message(FATAL_ERROR "Invalid value for FIDGETY_QT_MAJOR: ${FIDGETY_QT_MAJOR}. \
Fidgety only targets Qt6 or Qt5 (legacy)")
endif()
message(STATUS "Using Qt${FIDGETY_QT_MAJOR} to build Fidgety")
configure_file(${FIDGETY_CONFIG_HEADER_IN} ${FIDGETY_CONFIG_HEADER_OUT})

find_package(spdlog REQUIRED)
find_package(nlohmann_json REQUIRED)

#include_directories(include)
#file(GLOB_RECURSE fidgety_headers "include/*")
fidgety_add_my_library(FidgetyHeaders INTERFACE)
# include everything under the include/ folder
target_include_directories(
    FidgetyHeaders INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
if(FIDGETY_MASTER_PROJECT)
    install(DIRECTORY include/ DESTINATION include FILES_MATCHING PATTERN *.hpp)
    install(DIRECTORY include/ DESTINATION include FILES_MATCHING PATTERN *.h)
endif()
fidgety_install_library(FidgetyHeaders fidgety_headers_config.cmake)

add_subdirectory(src)

set(FIDGETY_BUILD_TESTING ON CACHE BOOL "whether to build fidgety's tests")
set(
    FIDGETY_GTEST_FROM_SOURCE ON
    CACHE BOOL "whether to download googletest and build it from source (recommended for windows)"
)
if(FIDGETY_MASTER_PROJECT)
    include(CTest)
endif()
if((FIDGETY_MASTER_PROJECT OR FIDGETY_BUILD_TESTING) AND BUILD_TESTING)
    message(STATUS "enabling testing")
    # googletest stuff
    include(cmake/googletest_include.cmake)
    # test directory
    message(STATUS "adding 'tests' subdirectory")
    add_subdirectory(tests)
endif()
