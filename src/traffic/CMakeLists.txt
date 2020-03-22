set(SOURCE_FILES
        BasicSimulation.cpp)

# define target
add_library(traffic_simulation STATIC ${SOURCE_FILES})

# include directories
target_include_directories(traffic_simulation PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        )