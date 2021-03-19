# create executable
add_executable(TrafficSimulationTest
        VehicleModelTest.cpp
        TrafficSimulationTest.cpp
        )

# create executable
add_executable(UnitInterfaceTest
        UnitInterfaceTest.cpp
        )

# link libs
target_link_libraries(UnitInterfaceTest PRIVATE
        simtraffic
        )

# add test
add_gtest(TrafficSimulationTest)
add_gtest(UnitInterfaceTest)