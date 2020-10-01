# ------------------------------------------------------------------------------
# CUCUMBER
# ------------------------------------------------------------------------------

# option to build gtest
option(ENABLE_CUCUMBER "Enables cucumber as testing framework" ON)

if (ENABLE_CUCUMBER)

    # message
    message(STATUS "Cucumber framework enabled")

    # set YAML_BUILD_SHARED_LIBS
    set(CUKE_ENABLE_BOOST_TEST OFF CACHE BOOL "Set option for submodule")
    set(CUKE_USE_STATIC_BOOST OFF CACHE BOOL "Set option for submodule")
    set(CUKE_ENABLE_QT OFF CACHE BOOL "Set option for submodule")
    set(CUKE_TESTS_E2E OFF CACHE BOOL "Set option for submodule")
    set(CUKE_TESTS_UNIT OFF CACHE BOOL "Set option for submodule")
    set(CUKE_USE_STATIC_GTEST ON CACHE BOOL "Set option for submodule")

    # set lib variables
    set(cucumber_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/lib/cucumber-cpp/include)

    # add yaml library
    add_subdirectory(${PROJECT_SOURCE_DIR}/lib/cucumber-cpp)

endif (ENABLE_CUCUMBER)