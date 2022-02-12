cmake_minimum_required(VERSION 3.10)
project(Fidgety)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)
find_package(spdlog REQUIRED)
find_package(nlohmann_json REQUIRED)

include_directories(include)

add_subdirectory(src)
