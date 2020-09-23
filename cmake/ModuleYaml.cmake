
# message
message(STATUS "Yaml processor enabled")

# set YAML_BUILD_SHARED_LIBS
set(YAML_BUILD_SHARED_LIBS OFF CACHE BOOL "Set option for submodule")
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "Set option for submodule")
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "Set option for submodule")
set(YAML_CPP_INSTALL OFF CACHE BOOL "Set option for submodule")
set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "Set option for submodule")

# set lib variables
set(yaml_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/lib/yaml-cpp/include)

# add yaml library
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/yaml-cpp)