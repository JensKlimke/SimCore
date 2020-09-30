# set sources
set(SOURCE_FILES
        TrafficSimulation.cpp
        )

# library
add_library(simtraffic STATIC ${SOURCE_FILES})

# include directories
target_include_directories(simtraffic PRIVATE
        ${PROJECT_SOURCE_DIR}/src/include
        )