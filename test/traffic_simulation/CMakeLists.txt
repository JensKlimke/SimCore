# create executable
add_executable(TrafficSimulationTest
        TrafficSimulationTest.cpp
        )

# link libs
target_link_libraries(TrafficSimulationTest PRIVATE
        traffic_simulation
        )

# include directories
target_include_directories(TrafficSimulationTest PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        )

# add test
add_gtest(TrafficSimulationTest)