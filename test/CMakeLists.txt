# option for advanced functionality
option(TEST_ADVANCED_FUNCTIONALITY "Enables or disables tests for advanced functionality" OFF)
option(CUCUMBER_SCENARIO_FUNCTIONALITY "Enables or disables the cucumber functionality" OFF)

# add sub directories
add_subdirectory(simcore)
add_subdirectory(server)
add_subdirectory(application)
add_subdirectory(traffic)

# cucumber
if(CUCUMBER_SCENARIO_FUNCTIONALITY)
    add_subdirectory(scenario)
endif(CUCUMBER_SCENARIO_FUNCTIONALITY)

# advanced functionality
if(TEST_ADVANCED_FUNCTIONALITY)
    add_subdirectory(configuration)
    add_subdirectory(blocks)
endif(TEST_ADVANCED_FUNCTIONALITY)