# create executable
add_executable(TrafficSimulationTest
        #TrafficSimulationTest.cpp
        #UnitInterfaceTest.cpp
        TrafficUnitTest.cpp
        VehicleModelTest.cpp
        )

# include directories
target_include_directories(TrafficSimulationTest PRIVATE
        ${PROJECT_SOURCE_DIR}/src/include) # library include directory

# add test
add_gtest(TrafficSimulationTest)