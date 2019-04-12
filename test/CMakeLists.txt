set(SOURCE_FILES
        main.cpp
        SimTest.h
        VehicleModelTest.h
        DMAlgoTest.h
        helpers/TestSimulation.h
        helpers/AgentTestParameters.h
        agent_tests/AMT_PriInputs.h
        agent_tests/AMT_VelControl.h
        agent_tests/AMT_VelChoice.h
        agent_tests/AMT_Stop.h
        agent_tests/AMT_Follow.h
        AlgoTest.h)

if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
else()
    message(WARNING "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif()

add_executable(sim_test ${SOURCE_FILES})
target_include_directories(sim_test PUBLIC ../lib ../src)
target_link_libraries(sim_test core models ${CONAN_LIBS})