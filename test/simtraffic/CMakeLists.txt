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

# include directories
target_include_directories(TrafficSimulationTest PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        )

# add test
add_gtest(TrafficSimulationTest)