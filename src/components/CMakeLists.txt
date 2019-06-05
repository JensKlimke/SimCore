set(SOURCE_FILES
        traffic/VehicleModel.cpp
        traffic/Agent.cpp
        traffic/Environment.cpp
        traffic/PrimaryController.cpp)

add_library(components STATIC ${SOURCE_FILES})
target_link_libraries(components PUBLIC core simmap curl)
target_include_directories(components PUBLIC ${CMAKE_SOURCE_DIR}/src)