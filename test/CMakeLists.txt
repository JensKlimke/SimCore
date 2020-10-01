# option for advanced functionality
option(TEST_ADVANCED_FUNCTIONALITY "Enables or disables tests for advanced functionality" OFF)
option(CUCUMBER_SCENARIO_FUNCTIONALITY "Enables or disables the cucumber functionality" OFF)
option(REMOTE_FUNCTIONALITY "Enables or disables the remote functionalities" ON)

# add sub directories
add_subdirectory(simcore)
add_subdirectory(simtraffic)
add_subdirectory(cucumber)

# server functionality
if(REMOTE_FUNCTIONALITY)
    add_subdirectory(server)
endif(REMOTE_FUNCTIONALITY)

# advanced functionality
if(TEST_ADVANCED_FUNCTIONALITY)
    add_subdirectory(blocks)
endif(TEST_ADVANCED_FUNCTIONALITY)