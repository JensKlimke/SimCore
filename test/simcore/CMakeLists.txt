# source files
set(SOURCE_FILES
        SimBasicTest.cpp
        SimTest.cpp
        SignalTest.cpp
        DataTest.cpp
        PlotTest.cpp
    )

# executable
add_executable(SimCoreTest ${SOURCE_FILES})

# include directories
target_include_directories(SimCoreTest PRIVATE
        ${PROJECT_SOURCE_DIR}/src/include ) # library include directory

# add tests
add_gtest(SimCoreTest)
