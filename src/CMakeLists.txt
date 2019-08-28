add_subdirectory(socket)

set(SOURCE_FILES
        core/Loop.cpp
        traffic/VehicleModel.cpp
        traffic/Agent.cpp
        traffic/Environment.cpp
        traffic/PrimaryController.cpp)

add_library(simcore STATIC ${SOURCE_FILES})
target_link_libraries(simcore PUBLIC simmap websocket)
target_include_directories(simcore PUBLIC ./)
