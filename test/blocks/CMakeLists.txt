# source files
set(SOURCE_FILES
        BlockTest.cpp
        SignalTest.cpp
    )

# add debug definition
add_definitions(-DDEBUG)

# executable
add_executable(BlocksTest ${SOURCE_FILES})

# include directories
target_include_directories(BlocksTest PRIVATE
        ${PROJECT_SOURCE_DIR}/include) # library include directory

# add tests
add_gtest(BlocksTest)