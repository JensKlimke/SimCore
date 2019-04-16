cmake_minimum_required(VERSION 3.5)
project(core CXX)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        IModel.h
        Loop.cpp Loop.h
        Exceptions.h
        ITimer.h
        IStopCondition
        IParameterizable.h
        IStorable.h
        DataManager.h
        ISynchronized.h
        functions.h
        IPlugin.h
        Plugin.h )

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ../)