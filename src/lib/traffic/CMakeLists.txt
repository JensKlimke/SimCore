# add custom target to build interfaces
set(IG_NODE_DIR ${PROJECT_SOURCE_DIR}/generator/src)
set(IG_MODELS_DIR ${PROJECT_SOURCE_DIR}/generator/models)
set(IG_TARGET_SRC_DIR ${PROJECT_SOURCE_DIR}/src/traffic)
set(IG_TARGET_HDR_DIR ${PROJECT_SOURCE_DIR}/include/simcore/traffic)

if(CREATE_INTERFACE_GENERATOR_TARGETS)

    add_custom_target(gen_interface_unit node ${IG_NODE_DIR}/generator.js ${IG_MODELS_DIR}/unit/config.json
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            BYPRODUCTS
            ${IG_TARGET_HDR_DIR}/UnitInterface.h
            ${IG_TARGET_HDR_DIR}/UnitData.h
            ${IG_TARGET_SRC_DIR}/UnitData.cpp
            SOURCES
            ${IG_MODELS_DIR}/unit/config.json
            ${IG_MODELS_DIR}/unit/UnitData.cpp.mustache
            ${IG_MODELS_DIR}/unit/UnitData.h.mustache
            ${IG_MODELS_DIR}/unit/UnitInterface.h.mustache
            COMMENT "Generating unit interface"
            )

endif()

# set source files
set(SOURCE_FILES
        BasicSimulation.cpp
        UnitData.cpp
        Unit.cpp)


# define target
add_library(traffic_simulation STATIC ${SOURCE_FILES})

# include directories
target_include_directories(traffic_simulation PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        )