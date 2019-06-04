cmake_minimum_required(VERSION 3.5)

project(SimCore CXX)
set(CMAKE_CXX_STANDARD 14)

# add ./cmake to CMAKE_MODULE_PATH
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# include sim map lib and provide path to source (for tracks)
include(AddSimMap)
add_definitions(-DSIMMAP_SRC=\"${simmap_SOURCE_DIR}\")

# include folders
include_directories(src/core)
include_directories(src/components)

# add source folder
add_subdirectory(src)

# first check
if(NOT DEFINED SIMCORE_ENABLE_TESTING)
    set(SIMCORE_ENABLE_TESTING true)
endif(NOT DEFINED SIMCORE_ENABLE_TESTING)

if(SIMCORE_ENABLE_TESTING)

    enable_testing()
    include(AddGoogleTest)

    add_subdirectory(test)

endif()