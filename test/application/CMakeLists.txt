# create executable
add_executable(ApplicationTestingTest
        ApplicationTest.cpp
        )

# include directories
target_include_directories(ApplicationTestingTest PRIVATE
        ${yaml_INCLUDE_DIR}
        )

# link libraries
target_link_libraries(ApplicationTestingTest PRIVATE
        yaml-cpp
        SimulationFramework
        )

# add test
add_gtest(ApplicationTestingTest)