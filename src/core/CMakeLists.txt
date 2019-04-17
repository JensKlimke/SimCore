cmake_minimum_required(VERSION 3.5)
project(core CXX)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        Loop.cpp
        DataNode.h
        )

set(HEADER_FILES
        IComponent.h
        Loop.h
        Exceptions.h
        ITimer.h
        IStopCondition
        IParameterizable.h
        IStorable.h
        DataManager.h
        ISynchronized.h
        functions.h
        IPlugin.h
        Plugin.h SimModel.h)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES})

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(FILES ${HEADER_FILES} DESTINATION include)
