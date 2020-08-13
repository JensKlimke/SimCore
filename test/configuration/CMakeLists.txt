# source files
set(SOURCE_FILES
        ConfigurationTest.cpp
    )

# executable
add_executable(ConfigurationTest ${SOURCE_FILES})

# include directories
target_include_directories(ConfigurationTest PRIVATE
        ${PROJECT_SOURCE_DIR}/src/include ) # library include directory

# add tests
add_gtest(ConfigurationTest)
