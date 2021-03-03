# define target
add_library(simtraffic STATIC
        BasicSimulation.cpp
        UnitData.cpp
        Unit.cpp
        )

# set header properties
set_target_properties(simtraffic PROPERTIES PUBLIC_HEADER
        ../../include/simtraffic/*
        )

# include directories
target_include_directories(simtraffic PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        )


# installation
install(TARGETS simtraffic
        PUBLIC_HEADER
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/myproj
        )
