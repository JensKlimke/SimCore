# create executable
add_executable(TrafficSimulationTest
        VehicleModelTest.cpp
        TrafficSimulationTest.cpp
        UnitInterfaceTest.cpp
        )

# link libs
target_link_libraries(TrafficSimulationTest PRIVATE
        simtraffic
        )

# add test
add_gtest(TrafficSimulationTest)