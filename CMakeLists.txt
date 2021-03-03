cmake_minimum_required(VERSION 3.10)

project(SimCore CXX)
set(CMAKE_CXX_STANDARD 14)

# set options
option(BUILD_TESTING "Sets or unsets the option to generate the test target" OFF)
option(ENABLE_COVERAGE "Sets or unsets the option to generate with coverage flags" OFF)

# add ./cmake to CMAKE_MODULE_PATH
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# for installation
include(GNUInstallDirs)

# set include directory
include_directories(include)
include_directories(lib/json/include)

# add modules
include(ModuleCoverage)
include(ModuleTesting)

# add sources
add_subdirectory(src/traffic)