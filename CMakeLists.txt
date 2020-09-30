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

# testing
include(ModuleTesting)


# sub directories
set(CUKE_ENABLE_BOOST_TEST OFF)
set(CUKE_ENABLE_QT OFF)
set(CUKE_TESTS_E2E OFF)
set(CUKE_TESTS_UNIT OFF)
set(CUKE_USE_STATIC_GTEST OFF)
add_subdirectory(lib/cucumber-cpp)
add_subdirectory(src/simcore/simtraffic)
