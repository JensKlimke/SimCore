cmake_minimum_required(VERSION 3.5)
project(sim_test CXX)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        main.cpp
        SimTest.h
        SignalTest.h ReporterTest.h)

if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
else()
    message(WARNING "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif()

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ../src)
target_link_libraries(${PROJECT_NAME} core models ${CONAN_LIBS})