cmake_minimum_required(VERSION 3.14)
project(Fidgety)

##### INCLUDES #####

include(GNUInstallDirs)
include(FetchContent)
include(cmake/add_my_library.cmake)
include(cmake/add_dependencies.cmake)

#~~~~ INCLUDES ~~~~#

##### PROJECT BUILD OPTIONS #####

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(FIDGETY_MASTER_PROJECT ON)
else()
    set(FIDGETY_MASTER_PROJECT OFF)
endif()

set(
    FIDGETY_BUILD_EXTENSIONS ${FIDGETY_MASTER_PROJECT}
    CACHE BOOL "Whether to build the extensions used by the fidgety editor"
)
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
    FIDGETY_GTEST_FROM_SOURCE ON
    CACHE BOOL "whether to download googletest and build it from source (recommended for windows)"
)
set(
    FIDGETY_GOOGLETEST_RELEASE_URL
    "https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip"
    CACHE STRING "The URL of the release of googletest to use"
)
set(
    FIDGETY_FMTLIB_FROM_SOURCE OFF
    CACHE BOOL "whether to download fmtlib/fmt and build it from source"
)
set(
    FIDGETY_FMTLIB_RELEASE_URL
    "https://github.com/fmtlib/fmt/archive/refs/tags/8.1.1.zip"
    CACHE STRING "The URL of the release of fmt to use"
)
set(
    FIDGETY_SPDLOG_FROM_SOURCE OFF
    CACHE BOOL "whether to download spdlog and build it from source (recommended for windows)"
)
set(
    FIDGETY_SPDLOG_RELEASE_URL
    "https://github.com/gabime/spdlog/archive/refs/tags/v1.9.2.zip"
    CACHE STRING "The URL of the release of spdlog to use"
)
set(
    FIDGETY_NLOHMANNJSON_FROM_SOURCE OFF CACHE BOOL
    "whether to download nlohmann/json and build it from source (recommended for windows)"
)
set(
    FIDGETY_NLOHMANNJSON_RELEASE_URL
    "https://github.com/nlohmann/json/archive/refs/tags/v3.10.5.zip"
    CACHE STRING "The URL of the release of nlohmann/json to use"
)
#set(
#    FIDGETY_BOOST_FROM_SOURCE OFF CACHE BOOL
#    "whether to download Boost and build it from source (not recommended for mental health)"
#)
#set(
#    FIDGETY_BOOST_RELEASE_URL
#    "https://github.com/boostorg/boost/archive/refs/tags/boost-1.78.0.zip"
#    CACHE STRING "The URL of the release of Boost to use"
#)
set(FIDGETY_BOOST_MINVER 1.67.0 CACHE STRING "Minimum version of Boost")
set(
    FIDGETY_QT_FROM_SOURCE OFF
    CACHE BOOL "whether to download Qt and build it from source"
)
set(FIDGETY_QT_MAJOR 6 CACHE STRING "The major version number of Qt being used to build Fidgety")
set(
    FIDGETY_QT_REPO_URL
    "https://code.qt.io/qt/qtbase.git"
    CACHE STRING "The URL to the git repository containing the source code of qtbase"
)
if(${FIDGETY_QT_FROM_SOURCE})
    if(FIDGETY_QT_MAJOR STREQUAL 6)
        set(
            FIDGETY_QT_REPO_TAG "0145fe008894c3b280649b02725e6ec5a5605006"
            CACHE STRING "The tag in the Qt source tree to use"
        )
    elseif(FIDGETY_QT_MAJOR STREQUAL 5)
        set(
            FIDGETY_QT_REPO_TAG "f5bcf9b017a84c08983d8e8d00d7177ecd6bbd2e"
            CACHE STRING "The tag in the Qt source tree to use"
        )
    else()
        set(
            FIDGETY_QT_REPO_TAG ""
            CACHE STRING "The tag in the Qt source tree to use"
        )
    endif()
endif()
set(
    FIDGETY_EXCEPTION_WHAT_USE_DESCRIPTION OFF CACHE BOOL
    "Whether to use Exception::getLongDescription(void) for Exception::what(void). \
This may cause memory leaks if turned on. \
If this option is turned off (which it is by default), Exception::what(void) \
will used a constant char[] expression instead."
)
set(FIDGETY_FMTLIB_HEADER_ONLY ON CACHE BOOL "Whether to use the header-only version of fmt")
set(FIDGETY_BUILD_TESTING ON CACHE BOOL "Whether to build fidgety's tests")
set(FIDGETY_TEST_LOGLEVEL "info" CACHE STRING "Spdlog level to use when running tests")
set(MICROSOFT_BULLSHIT OFF CACHE BOOL "Fix for intellisense. Do not use for release")

#~~~~ PROJECT BUILD OPTIONS ~~~~#

##### CMAKE SETTINGS #####

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 90)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#~~~~ CMAKE SETTINGS ~~~~#

##### CONFIG.H.IN #####

set(FIDGETY_CONFIG_HEADER_IN "${PROJECT_SOURCE_DIR}/include/fidgety/config.h.in")
set(FIDGETY_CONFIG_HEADER_OUT "${PROJECT_SOURCE_DIR}/include/fidgety/config.h")
configure_file(${FIDGETY_CONFIG_HEADER_IN} ${FIDGETY_CONFIG_HEADER_OUT})

#~~~~ CONFIG.H.IN ~~~~#

##### BUILD DEPENDENCIES #####

if(${FIDGETY_QT_FROM_SOURCE})
    fidgety_add_dependency_sourced_git(Qt ${FIDGETY_QT_REPO_URL} ${FIDGETY_QT_REPO_TAG})
else()
    if(FIDGETY_QT_MAJOR STREQUAL 6)
        find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)
        set(FIDGETY_QT_CORE Qt6::Core)
        set(FIDGETY_QT_GUI Qt6::Gui)
        set(FIDGETY_QT_WIDGETS Qt6::Widgets)
    elseif(FIDGETY_QT_MAJOR STREQUAL 5)
        find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
        set(FIDGETY_QT_CORE Qt5::Core)
        set(FIDGETY_QT_GUI Qt5::Gui)
        set(FIDGETY_QT_WIDGETS Qt5::Widgets)
    else()
        message(FATAL_ERROR "Invalid value for FIDGETY_QT_MAJOR: ${FIDGETY_QT_MAJOR}. \
Fidgety only targets Qt6 or Qt5 (legacy)")
    endif()
    message(STATUS "Using Qt${FIDGETY_QT_MAJOR} to build Fidgety")
endif()

set(Boost_USE_STATIC_LIBS        ON)  # only find static libs
set(Boost_USE_DEBUG_LIBS        OFF)  # ignore debug libs and
set(Boost_USE_RELEASE_LIBS       ON)  # only find release libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(
    Boost ${FIDGETY_BOOST_MINVER} REQUIRED COMPONENTS
    filesystem regex
)
if(NOT Boost_FOUND)
    message(FATAL_ERROR "Boost not found")
endif()

if(${FIDGETY_FMTLIB_FROM_SOURCE})
    set(FMT_INSTALL ON)
    fidgety_add_dependency_sourced_url(fmt ${FIDGETY_FMTLIB_RELEASE_URL})
else()
    fidgety_add_dependency_installed(fmt)
endif()
if(${FIDGETY_SPDLOG_FROM_SOURCE})
    set(SPDLOG_INSTALL ON)
    fidgety_add_dependency_sourced_url(spdlog ${FIDGETY_SPDLOG_RELEASE_URL})
else()
    fidgety_add_dependency_installed(spdlog)
endif()
if(${FIDGETY_NLOHMANNJSON_FROM_SOURCE})
    set(JSON_Install ON)
    fidgety_add_dependency_sourced_url(nlohmann_json ${FIDGETY_NLOHMANNJSON_RELEASE_URL})
    include_directories(${CMAKE_BINARY_DIR}/_deps/nlohmann_json-src/include)
else()
    fidgety_add_dependency_installed(nlohmann_json)
endif()

#~~~~ BUILD DEPENDENCIES ~~~~#

##### FIDGETY HEADERS #####

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

#~~~~ FIDGETY HEADERS ~~~~#

add_subdirectory(src)

##### TESTING #####

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

#~~~~ TESTING ~~~~#
