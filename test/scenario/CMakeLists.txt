# find gtest and boost
find_package(GTest REQUIRED)
find_package(Boost 1.45.0 COMPONENTS program_options filesystem regex)

# executable
add_executable(ScenarioSteps features/step_definitions/GTestScenarioSteps)

# link libraries
target_link_libraries(ScenarioSteps PRIVATE
        SimulationFramework
        cucumber-cpp
        GTest::GTest
        ${Boost_LIBRARIES})