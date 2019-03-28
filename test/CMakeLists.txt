set(SOURCE_FILES
        main.cpp
        SimTest.h
        VehicleModelTest.h
        AgentModelTest.h
        helpers/TestSimulation.h
        ModelTest.h
        helpers/AgentStandardParameters.h
        )


enable_testing()

add_executable(sim_test ${SOURCE_FILES})
target_include_directories(sim_test PUBLIC ../lib ../src)
target_link_libraries(sim_test gtest core models ${CMAKE_DL_LIBS})