cmake_minimum_required(VERSION 3.0)
project(simulation-framework)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(lib/googletest)
add_subdirectory(src/core)
add_subdirectory(src/models)
add_subdirectory(test)