cmake_minimum_required(VERSION 3.0)
project(SimCore CXX)

set(CMAKE_CXX_STANDARD 14)

enable_testing()

add_subdirectory(src/core)
add_subdirectory(src/models)
add_subdirectory(test)