# create executable
add_executable(ApplicationTestingTest
        ApplicationTest.cpp
        )

# add current folder for config files
target_compile_definitions(ApplicationTestingTest PRIVATE
        CONFIG_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

# link libraries
target_link_libraries(ApplicationTestingTest PRIVATE SimulationFramework)

# add test
add_gtest(ApplicationTestingTest)