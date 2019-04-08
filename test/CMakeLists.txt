set(SOURCE_FILES
        main.cpp
        SimTest.h
        VehicleModelTest.h
        AlgoTest.h
        helpers/TestSimulation.h
        helpers/AgentStandardParameters.h
        agent/AMT_PriInputs.h
        agent/AMT_VelControl.h
        agent/AMT_VelChoice.h agent/AMT_Stop.h)


enable_testing()

add_executable(sim_test ${SOURCE_FILES})
target_include_directories(sim_test PUBLIC ../lib ../src)
target_link_libraries(sim_test gtest core models ${CMAKE_DL_LIBS})