# ------------------------------------------------------------------------------
# CUCUMBER
# ------------------------------------------------------------------------------

# option to build gtest
option(ENABLE_CUCUMBER "Enables cucumber as testing framework" ON)

if (ENABLE_CUCUMBER)

    # message
    message(STATUS "Cucumber framework enabled")

    # set YAML_BUILD_SHARED_LIBS
    set(CUKE_ENABLE_BOOST_TEST OFF)
    set(CUKE_USE_STATIC_BOOST OFF)
    set(CUKE_ENABLE_QT OFF)
    set(CUKE_TESTS_E2E OFF)
    set(CUKE_TESTS_UNIT OFF)
    set(CUKE_ENABLE_GTEST OFF)

    # set lib variables
    set(cucumber_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/lib/cucumber-cpp/include)

    # add yaml library
    add_subdirectory(${PROJECT_SOURCE_DIR}/lib/cucumber-cpp)

endif (ENABLE_CUCUMBER)