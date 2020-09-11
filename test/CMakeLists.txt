# option for advanced functionality
option(TEST_ADVANCED_FUNCTIONALITY "Enables or disables tests for advanced functionality" OFF)

# add sub directories
add_subdirectory(simcore)
add_subdirectory(server)
add_subdirectory(application)
add_subdirectory(traffic)

# advanced functionality
if(TEST_ADVANCED_FUNCTIONALITY)
    add_subdirectory(configuration)
    add_subdirectory(blocks)
endif(TEST_ADVANCED_FUNCTIONALITY)