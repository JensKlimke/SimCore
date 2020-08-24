cmake_minimum_required(VERSION 3.10)

project(SimCore CXX)
set(CMAKE_CXX_STANDARD 17)

# set options
option(BUILD_TESTING "Sets or unsets the option to generate the test target" OFF)
option(ENABLE_COVERAGE "Enables the coverage check of the module" OFF)
option(CREATE_DOXYGEN_TARGET "Enable to create a doxygen target" OFF)
option(BUILD_SIMTEST "Sets or unsets the option to generate the test target" OFF)

# add ./cmake to CMAKE_MODULE_PATH
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# Must use GNUInstallDirs to install libraries into correct locations on all platforms.
include(GNUInstallDirs)

# yaml library
include(ModuleYaml)


# ------------------------------------------------------------------------------
# DOCUMENTATION
# ------------------------------------------------------------------------------
if(CREATE_DOXYGEN_TARGET)

    # exclude pattern
    set(DOXYGEN_EXCLUDE_PATTERN "")

    # load module for doxygen target
    include(ModuleDoxygen)

endif(CREATE_DOXYGEN_TARGET)



# ------------------------------------------------------------------------------
# COVERAGE
# ------------------------------------------------------------------------------
if (ENABLE_COVERAGE)

    # load module for coverage
    include(ModuleCoverage)

endif (ENABLE_COVERAGE)


# ------------------------------------------------------------------------------
# SOCKET
# ------------------------------------------------------------------------------

# build libraries
add_subdirectory(src/lib/socket)
add_subdirectory(src/lib/test)


# ------------------------------------------------------------------------------
# TESTING
# ------------------------------------------------------------------------------
if(BUILD_TESTING)

    # load module for testing
    include(ModuleGtest)

endif(BUILD_TESTING)