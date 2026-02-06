cmake_minimum_required(VERSION 3.14)

project(SimCore CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set options
option(BUILD_TESTS "Sets or unsets the option to generate the test target" OFF)
option(BUILD_FOR_COVERAGE "Sets or unsets the option to generate with coverage flags" OFF)
option(BUILD_TRAFFIC_SIMULATION "Activates or deactivates the traffic simulation library target to be generated." ON)

# add ./cmake to CMAKE_MODULE_PATH
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# fetch nlohmann/json
include(FetchContent)
FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG        v3.11.3
        GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(json)

# header-only interface library for SimCore
add_library(simcore_headers INTERFACE)
target_include_directories(simcore_headers INTERFACE
        ${PROJECT_SOURCE_DIR}/include
        )
target_link_libraries(simcore_headers INTERFACE nlohmann_json::nlohmann_json)

# code coverage
if (BUILD_FOR_COVERAGE)

    message(STATUS "Coverage option is enabled")

    if (CMAKE_COMPILER_IS_GNUCXX)
        set(CMAKE_CXX_FLAGS "--coverage")
    elseif ("${CMAKE_C_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang"
            OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang")
        set(CMAKE_CXX_FLAGS "-fprofile-instr-generate -fcoverage-mapping")
    endif ()

endif ()


# traffic simulation target
if(BUILD_TRAFFIC_SIMULATION)

    message(STATUS "Traffic simulation target enabled")

    # add sources
    add_subdirectory(src/traffic)

endif()


# testing
if(BUILD_TESTS)

    message(STATUS "Testing of SimCore enabled")

    enable_testing()

    # fetch GTest
    include(FetchContent)
    FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG        v1.14.0
            GIT_SHALLOW    TRUE
    )
    FetchContent_MakeAvailable(googletest)

    # load macros for tests
    include(AddGoogleTest)

    # add test folder
    add_subdirectory(test/simcore)

    # tests for traffic simulation
    if(BUILD_TRAFFIC_SIMULATION)
        add_subdirectory(test/traffic_simulation)
    endif()

endif()
