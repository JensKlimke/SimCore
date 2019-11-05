cmake_minimum_required(VERSION 3.11)

project(SimCore CXX)
set(CMAKE_CXX_STANDARD 14)

# add ./cmake to CMAKE_MODULE_PATH
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# download gtest when enabled
if(NOT DEFINED SIMCORE_ENABLE_TESTING)
    set(SIMCORE_ENABLE_TESTING true)
endif(NOT DEFINED SIMCORE_ENABLE_TESTING)

if(SIMCORE_ENABLE_TESTING)

    # enable testing
    enable_testing()

    # add subdirectory, set include path and include gtest functions
    add_subdirectory(lib/gtest)
    include_directories(lib/gtest/googletest/include)
    include(AddGoogleTest)

    # add tests
    add_subdirectory(test)

endif()

# include folders
include_directories(src/core src/components src/socket)

#find_package(Boost COMPONENTS program_options REQUIRED )
find_package(Boost 1.69)

# add source folder
add_subdirectory(src)
