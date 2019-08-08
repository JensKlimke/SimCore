cmake_minimum_required(VERSION 3.11)

project(SimCore CXX)
set(CMAKE_CXX_STANDARD 14)

# add ./cmake to CMAKE_MODULE_PATH
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# include sim map lib and provide path to source (for tracks)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(SIMMAP_ENABLE_TESTING OFF CACHE BOOL "" FORCE)

add_subdirectory(lib/SimMap)
include_directories(lib/SimMap/src)
add_definitions(-DSIMMAP_SRC=\"lib/SimMap\")

# download gtest when enabled
if(NOT DEFINED SIMCORE_ENABLE_TESTING)
    set(SIMCORE_ENABLE_TESTING true)
endif(NOT DEFINED SIMCORE_ENABLE_TESTING)

if(SIMCORE_ENABLE_TESTING)
    enable_testing()
    include(AddGoogleTest)
endif()

# include folders
include_directories(src/core src/components src/socket)

#find_package(Boost COMPONENTS program_options REQUIRED )
find_package(Boost 1.69)
if(NOT Boost_FOUND)
    include(AddBoost)
endif()

if(SIMCORE_ENABLE_TESTING)
    add_subdirectory(test)
endif()

# add source folder
add_subdirectory(src)
