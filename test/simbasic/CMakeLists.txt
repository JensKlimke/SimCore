# create executable
add_executable(BasicTest
        SplineTest.cpp
        )

# add test
add_gtest(BasicTest)