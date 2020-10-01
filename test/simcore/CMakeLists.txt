# source files
set(SOURCE_FILES
        SimulationTestTest.cpp
        SynchronizedTimerTest.cpp
        StopConditionTest.cpp
        SignalBasicTest.cpp
        SignalTest.cpp
        DataTest.cpp
        LoggingTest.cpp
        # PlotTest.cpp
    )

# executable
add_executable(SimCoreTest ${SOURCE_FILES})

# include directories
target_include_directories(SimCoreTest PRIVATE
        ${PROJECT_SOURCE_DIR}/src/include
        )

# add tests
add_gtest(SimCoreTest)
