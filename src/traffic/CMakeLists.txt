# set source files
set(SOURCE_FILES
        BasicSimulation.cpp
        UnitData.cpp
        Unit.cpp)

# define target
add_library(traffic_simulation STATIC ${SOURCE_FILES})

# link dependencies
target_link_libraries(traffic_simulation PUBLIC
        simcore_headers
        nlohmann_json::nlohmann_json
        )
