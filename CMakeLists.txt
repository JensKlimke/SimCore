cmake_minimum_required(VERSION 3.5)
project(SimCore CXX)

set(CMAKE_CXX_STANDARD 14)

# add ./cmake to CMAKE_MODULE_PATH
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

set(SIMMAP_ENABLE_TESTING false)

enable_testing()
include(AddGoogleTest)
include(AddSimMap)

add_subdirectory(src/core)
add_subdirectory(src/components)
add_subdirectory(test)

include_directories(src/)