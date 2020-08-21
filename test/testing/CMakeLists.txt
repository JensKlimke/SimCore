# source files
set(SOURCE_FILES
        TestingTest.cpp
    )

# executable
add_executable(TestingTest ${SOURCE_FILES})

# include directories
target_include_directories(TestingTest PRIVATE
        ${PROJECT_SOURCE_DIR}/src/include
        ) # library include directory

target_link_libraries(TestingTest PRIVATE
        simcore_test)

# add tests
add_gtest(TestingTest)
