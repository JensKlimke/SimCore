# create executable
add_executable(BasicTest
        SplineTest.cpp
        PolynomialTest.cpp
        )

# add test
add_gtest(BasicTest)