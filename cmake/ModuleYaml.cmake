
# message
message(STATUS "Yaml processor enabled")

# set YAML_BUILD_SHARED_LIBS
set(YAML_BUILD_SHARED_LIBS OFF CACHE BOOL "Set option for submodule")

# add yaml library
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/yaml-cpp)