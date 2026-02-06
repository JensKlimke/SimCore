# create executable
add_executable(TrafficSimulationTest
        TrafficSimulationTest.cpp
        UnitInterfaceTest.cpp
        )

# link libs (traffic_simulation transitively provides simcore_headers + nlohmann_json)
target_link_libraries(TrafficSimulationTest PRIVATE
        traffic_simulation
        )

# add test
add_gtest(TrafficSimulationTest)
