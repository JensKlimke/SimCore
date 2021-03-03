# define and create log dir
set(PROJECT_LOG_DIR ${PROJECT_SOURCE_DIR}/log)
file(MAKE_DIRECTORY ${PROJECT_LOG_DIR})
add_definitions(-DLOG_DIR=\"${PROJECT_LOG_DIR}\")


# add sources
add_executable(SimCoreTest
        SimTest.cpp
        SignalTest.cpp
        DataTest.cpp
        )


# add test
add_gtest(SimCoreTest)
