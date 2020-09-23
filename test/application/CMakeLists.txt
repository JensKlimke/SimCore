# create executable
add_executable(ApplicationTestingTest
        ApplicationTest.cpp
        )

# add current folder for config files
target_compile_definitions(ApplicationTestingTest PRIVATE
        CONFIG_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

# include directories
target_include_directories(ApplicationTestingTest PRIVATE
        ${PROJECT_SOURCE_DIR}/src/include
        ${PROJECT_SOURCE_DIR}/lib/yaml-cpp/include
        )

# link libraries
target_link_libraries(ApplicationTestingTest PRIVATE
        yaml-cpp
        )

# add test
add_gtest(ApplicationTestingTest)