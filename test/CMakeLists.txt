cmake_minimum_required(VERSION 3.5)
project(sim_test CXX)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        main.cpp
        SimTest.h
        SignalTest.h
        DataTest.h
        PluginTest.h
        EnvironmentTest.h
        PublishTest.h
        VehicleModelTest.h
        )

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} core components odrfw gtest)
