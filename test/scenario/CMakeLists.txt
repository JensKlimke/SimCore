# find gtest and boost
find_package(GTest REQUIRED)
find_package(Boost 1.45.0 COMPONENTS program_options filesystem regex)

# executable
add_executable(ScenarioSteps
        features/step_definitions/VirtualVehicleSteps.cpp
        )

# include library
target_include_directories(ScenarioSteps PRIVATE
        ${PROJECT_SOURCE_DIR}/src/include
        )

# link libraries
target_link_libraries(ScenarioSteps PRIVATE
        simtraffic
        cucumber-cpp
        GTest::GTest
        ${Boost_LIBRARIES})