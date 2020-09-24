# create executable
add_executable(ApplicationTestingTest
        ApplicationTest.cpp
        TrafficSimulation.cpp
        )

# add current folder for config files
target_compile_definitions(ApplicationTestingTest PRIVATE
        CONFIG_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
        )

target_include_directories(ApplicationTestingTest PRIVATE
        ${PROJECT_SOURCE_DIR}/src/include
        )

# add test
add_gtest(ApplicationTestingTest)