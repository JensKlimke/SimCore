# create executable
add_executable(SimulationTest
        SimulationTest.cpp
        )

# link libs
target_link_libraries(SimulationTest PRIVATE
        traffic_simulation
        )

# include directories
target_include_directories(SimulationTest PRIVATE
        ${PROJECT_SOURCE_DIR}/app
        ${PROJECT_SOURCE_DIR}/include
        )

# add test
add_gtest(SimulationTest)