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
include(ModuleCucumber) # cucumber framework

# testing
include(ModuleTesting)

# lib sources
add_subdirectory(src/socket)
add_subdirectory(src/storage)