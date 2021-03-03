# define target
add_library(simtraffic STATIC
        BasicSimulation.cpp
        UnitData.cpp
        Unit.cpp
        )

# include directories
target_include_directories(simtraffic PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        )

# install library
install(TARGETS simtraffic DESTINATION lib)