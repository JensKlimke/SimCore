cmake_minimum_required(VERSION 3.10)

project(SimCore CXX)
set(CMAKE_CXX_STANDARD 14)

# set options
option(BUILD_TESTS "Sets or unsets the option to generate the test target" OFF)
option(BUILD_GTEST "Sets or unsets the option to build gtest" OFF)
option(BUILD_FOR_COVERAGE "Sets or unsets the option to generate with coverage flags" OFF)
option(BUILD_WEBSOCKET "Activates or deactivates the websocket module of the SimCore" OFF)
option(BUILD_TRAFFIC_SIMULATION "Activates or deactivates the traffic simulation library target to be generated." OFF)

# add ./cmake to CMAKE_MODULE_PATH
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# set include directory
include_directories(${PROJECT_SOURCE_DIR}/include)


# code coverage
if (BUILD_FOR_COVERAGE)

    # message
    message("-- Coverage option is enabled")

    # gnu or clang
    if (CMAKE_COMPILER_IS_GNUCXX)
        set(CMAKE_CXX_FLAGS "--coverage")
    elseif ("${CMAKE_C_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang"
            OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang")
        set(CMAKE_CXX_FLAGS "-fprofile-instr-generate -fcoverage-mapping")
    endif ()

    # from: https://blog.jetbrains.com/clion/2019/10/clion-2019-3-eap-coverage-cmake-defaults-lldb/

endif (BUILD_FOR_COVERAGE)



# traffic simulation target
if(BUILD_TRAFFIC_SIMULATION)

    message("-- Traffic simulation target enabled")

    add_subdirectory(${PROJECT_SOURCE_DIR}/src/traffic)

endif(BUILD_TRAFFIC_SIMULATION)



## Enable google testing of SimCore
if(BUILD_TESTS)

    message("-- Testing of SimCore enabled")

    # enable testing
    enable_testing()

    # switch (build or use installed lib)
    if(BUILD_GTEST)

        # add subdirectory
        add_subdirectory(googletest)
        set(GTEST_BOTH_LIBRARIES gtest_main)
        include_directories(${PROJECT_SOURCE_DIR}/googletest/googletest/include)

    else()

        # find gtest
        find_package(GTest REQUIRED)
        include_directories(${GTEST_INCLUDE_DIRS})

    endif()

    # load macros for tests
    include(AddGoogleTest)

    # add test folder
    add_subdirectory(test/simcore)

    # tests for traffic simulation
    if(BUILD_TRAFFIC_SIMULATION)
        add_subdirectory(test/traffic_simulation)
    endif()

endif()



# websocket
if(BUILD_WEBSOCKET)

    message("-- Websocket functionality enabled")

    # find boost
    find_package(Boost 1.69)

    # include folders
    add_subdirectory(src/socket)

endif()

