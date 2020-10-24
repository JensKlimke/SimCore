if (SIMCORE_ENABLE_CUCUMBER)

    # target to execute the cucumber framework
    add_custom_target(RunScenarioSteps ALL
            ${CMAKE_CURRENT_BINARY_DIR}/ScenarioSteps &
            COMMAND cucumber features/VehicleModelLongitudinalMotion.feature
            DEPENDS ScenarioSteps
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Run the cucumber test framework"
            )

    # executable
    add_executable(ScenarioSteps
            features/step_definitions/VirtualVehicleSteps.cpp
            )

    # include library
    target_include_directories(ScenarioSteps PRIVATE
            ${PROJECT_SOURCE_DIR}/include
            )

    # link libraries
    target_link_libraries(ScenarioSteps PRIVATE
            cucumber-cpp
            )

endif (SIMCORE_ENABLE_CUCUMBER)