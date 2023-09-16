cmake_minimum_required(VERSION 3.21)
project(FrontierPhoenix)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

find_package(Boost 1.81.0)
find_package(GiNaC 1.8)

add_subdirectory(src)
add_subdirectory(tst)
