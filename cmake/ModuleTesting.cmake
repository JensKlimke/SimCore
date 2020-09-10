# ------------------------------------------------------------------------------
# TESTING
# usage: cmake flag BUILD_TESTS ON|OFF to enable or disable to build the
#        test targets
# ------------------------------------------------------------------------------

# options
option(BUILD_TESTS "Sets or unsets the option to generate the test target" OFF)

if(BUILD_TESTS)

    # load googletest
    include(ModuleGtest)

    # add test folder
    add_subdirectory(${PROJECT_SOURCE_DIR}/test)

endif(BUILD_TESTS)