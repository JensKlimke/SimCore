cmake_minimum_required(VERSION 3.5)
project(SimCore CXX)

set(CMAKE_CXX_STANDARD 14)

enable_testing()

add_subdirectory(src)
add_subdirectory(test)