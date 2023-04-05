cmake_minimum_required(VERSION 3.25)
project(compiler)

set(CMAKE_CXX_STANDARD 20)

add_library(compiler SHARED library.cpp)
add_executable(test1 test_1.cpp)
target_link_libraries(test1 gtest compiler)

add_executable(test2 test_2.cpp)
target_link_libraries(test2 compiler)