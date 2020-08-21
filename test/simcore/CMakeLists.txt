# source files
set(SOURCE_FILES
        SimTest.cpp
        SynchronizedTimerTest.cpp
        WebsocketTimerTest.cpp
        StopConditionTest.cpp
        SignalBasicTest.cpp
        SignalTest.cpp
        DataTest.cpp
        PlotTest.cpp
    )

# executable
add_executable(SimCoreTest ${SOURCE_FILES})

# include directories
target_include_directories(SimCoreTest PRIVATE
        ${PROJECT_SOURCE_DIR}/src/include ) # library include directory

# link libraries
target_link_libraries(SimCoreTest PRIVATE
        simcore_socket
        )

# add tests
add_gtest(SimCoreTest)
