cmake_minimum_required(VERSION 3.11)

project(SimCore CXX)
set(CMAKE_CXX_STANDARD 14)

# set options
option(SIMCORE_ENABLE_TESTING "Sets or unsets the option to generate the test target" ON)
option(SIMCORE_ENABLE_WEBSOCKET "Activates or deactivates the websocket module of the SimCore" ON)

# add ./cmake to CMAKE_MODULE_PATH
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# set binary directory
set(BIN_DIR ${PROJECT_SOURCE_DIR}/bin CACHE PATH "BIN_DIR")

# set include directory
include_directories(${PROJECT_SOURCE_DIR}/include)

## Enable google testing of SimCore
if(SIMCORE_ENABLE_TESTING)

    message("-- Testing of SimCore enabled")

    # enable testing
    enable_testing()

    # add subdirector (gtest)
    add_subdirectory(lib/gtest)

    # load macros for tests
    include(AddGoogleTest)

    # add test folder
    add_subdirectory(test)

endif()


if(SIMCORE_ENABLE_WEBSOCKET)

    # find boost
    find_package(Boost 1.69)

    # include folders
    add_subdirectory(src/socket)

endif()