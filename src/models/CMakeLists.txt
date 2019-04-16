cmake_minimum_required(VERSION 3.5)
project(models CXX)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        timers/BasicTimer.h timers/BasicTimer.cpp
        timers/TimeIsUp.h timers/TimeIsUp.cpp
        VehicleModel.h VehicleModel.cpp
        timers/RealTimeTimer.h timers/RealTimeTimer.cpp
        reporters/JsonReporter.h
        reporters/TimeReporter.h
        value/ValueExceed.h
        value/ValueOutOfTube.h
        value/SignalTube.h)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ../)