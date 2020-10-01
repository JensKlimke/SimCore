if (ENABLE_CUCUMBER)

    # executable
    add_executable(ScenarioSteps
            step_definitions/VirtualVehicleSteps.cpp
            )

    # include library
    target_include_directories(ScenarioSteps PRIVATE
            ${PROJECT_SOURCE_DIR}/src/include
            )

    # link libraries
    target_link_libraries(ScenarioSteps PRIVATE
            simtraffic
            cucumber-cpp
            ${GTEST_BOTH_LIBRARIES})

endif (ENABLE_CUCUMBER)