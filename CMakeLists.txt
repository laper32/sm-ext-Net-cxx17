cmake_minimum_required(VERSION 3.22)

project(sample_ext)

set(CMAKE_CXX_STANDARD 20)

include(${CMAKE_CURRENT_LIST_DIR}/configure.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/All.cmake)

find_package(fmt REQUIRED)
find_package(Boost REQUIRED)
#find_package(nlohmann_json REQUIRED)
find_package(OpenSSL REQUIRED)

add_subdirectory(src)