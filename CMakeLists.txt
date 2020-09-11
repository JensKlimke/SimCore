cmake_minimum_required(VERSION 3.10)

# project
project(SimCore CXX)
set(CMAKE_CXX_STANDARD 17)

# add ./cmake to CMAKE_MODULE_PATH
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# Must use GNUInstallDirs to install libraries into correct locations on all platforms.
include(GNUInstallDirs)

# load features
include(ModuleDoxygen)  # doxygen
include(ModuleCoverage) # code coverage
include(ModuleYaml)     # yaml parser

# library
option(BUILD_SIMCORE_LIBRARIES "Creates the library (for server connections needed)" OFF)

# add library code
if(BUILD_SIMCORE_LIBRARIES)

    message(STATUS "Building simcore libraries")

    add_subdirectory(${PROJECT_SOURCE_DIR}/src/lib/socket)

endif(BUILD_SIMCORE_LIBRARIES)


# testing
include(ModuleTesting)

