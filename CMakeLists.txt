cmake_minimum_required(VERSION 3.10)

# project
project(SimCore CXX)
set(CMAKE_CXX_STANDARD 17)

# option to build gtest
option(SIMCORE_ENABLE_SOCKET "Enables boost and all boost-dependent modules" ON)

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

# installation
INSTALL( DIRECTORY ./include/simcore DESTINATION include)

# boost-dependent sources
if (SIMCORE_ENABLE_SOCKET)
    add_subdirectory(src/socket)
endif (SIMCORE_ENABLE_SOCKET)