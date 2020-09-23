# find gtest and boost
find_package(GTest REQUIRED)
find_package(Boost 1.45.0 COMPONENTS program_options filesystem regex)

# library (to be replaced by test framework)
add_library(Scenario STATIC src/Simulation)
target_include_directories(Scenario INTERFACE src)

# Test executable
add_executable(ScenarioSteps features/step_definitions/GTestScenarioSteps)
target_link_libraries(ScenarioSteps PRIVATE Scenario cucumber-cpp GTest::GTest ${Boost_LIBRARIES})