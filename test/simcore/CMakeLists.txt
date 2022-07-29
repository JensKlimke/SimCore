# add sources
add_executable(SimCoreTest
        BasicSimulationTest.cpp
        LoopTest.cpp
        SimTest.cpp
#       StopConditionTest.cpp
        SignalTest.cpp
        LogginTest.cpp
        DataReporterTest.cpp
        )


# add test
add_gtest(SimCoreTest)
