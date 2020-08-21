# set sources
set(SOURCE_FILES
        Configurator.cpp
        Generator.cpp
        )

# library
add_library(simcore_test STATIC ${SOURCE_FILES})

# include directories
target_include_directories(simcore_test PRIVATE
        ${PROJECT_SOURCE_DIR}/src/include
        ${PROJECT_SOURCE_DIR}/lib/json/include
        ${PROJECT_SOURCE_DIR}/lib/yaml-cpp/include
        )

target_link_libraries(simcore_test PRIVATE
        yaml-cpp)