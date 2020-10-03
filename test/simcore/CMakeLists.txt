# source files
set(SOURCE_FILES
        TimerTest.cpp
        SynchronizedTimerTest.cpp
        StopConditionTest.cpp
        SignalBasicTest.cpp
        SignalTest.cpp
        SimulationTestTest.cpp
        DataTest.cpp
        LoggingTest.cpp
        StorageTest.cpp
    )

# executable
add_executable(SimCoreTest ${SOURCE_FILES})

# include directories
target_include_directories(SimCoreTest PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        )

# add tests
add_gtest(SimCoreTest)
