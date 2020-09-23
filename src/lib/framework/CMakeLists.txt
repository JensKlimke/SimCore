# library of the simulation test framework
add_library(SimulationFramework STATIC
        Simulation
        ModelManager
        )

# include directories
target_include_directories(SimulationFramework PUBLIC
        ../../include
        ${yaml_INCLUDE_DIR}
        )

# link libraries
target_link_libraries(SimulationFramework PUBLIC
        yaml-cpp
        )