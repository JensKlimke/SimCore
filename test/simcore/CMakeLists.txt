# add sources
add_executable(SimCoreTest
        LoopTest.cpp
        SimTest.cpp
#        StopConditionTest.cpp
#        SignalTest.cpp
        LogginTest.cpp
        )


# add test
add_gtest(SimCoreTest)
