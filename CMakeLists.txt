cmake_minimum_required(VERSION 3.10)

project(SimCore CXX)
set(CMAKE_CXX_STANDARD 14)

# set options
option(SIMCORE_BUILD_TESTS "Sets or unsets the option to generate the test target" OFF)
option(SIMCORE_BUILD_GTEST "Sets or unsets the option to build gtest" OFF)

# add ./cmake to CMAKE_MODULE_PATH
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# Must use GNUInstallDirs to install libraries into correct locations on all platforms.
include(GNUInstallDirs)



# ------------------------------------------------------------------------------
# DOCUMENTATION
# ------------------------------------------------------------------------------
if(SIMCORE_CREATE_DOXYGEN_TARGET)

    # exclude pattern
    set(DOXYGEN_EXCLUDE_PATTERN "")

    # load module for doxygen target
    include(ModuleDoxygen)

endif(SIMCORE_CREATE_DOXYGEN_TARGET)



# ------------------------------------------------------------------------------
# COVERAGE
# ------------------------------------------------------------------------------
if (SIMCORE_ENABLE_COVERAGE)

    # load module for coverage
    include(ModuleCoverage)

endif (SIMCORE_ENABLE_COVERAGE)


# ------------------------------------------------------------------------------
# TESTING
# ------------------------------------------------------------------------------
if(SIMCORE_BUILD_TESTS)

    # load module for testing
    include(ModuleGtest)

endif(SIMCORE_BUILD_TESTS)