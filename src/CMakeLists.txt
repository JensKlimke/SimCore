set(SOURCE_FILES
        core/Loop.cpp
        traffic/VehicleModel.cpp
        traffic/Agent.cpp
        traffic/Environment.cpp
        traffic/PrimaryController.cpp)

add_library(simcore STATIC ${SOURCE_FILES})

if(SIMCORE_ENABLE_SOCKET)

    add_subdirectory(socket)
    target_link_libraries(simcore PRIVATE simmap websocket)

else()

    target_link_libraries(simcore PRIVATE simmap)

endif()

target_include_directories(simcore PRIVATE ./)

set(simmap_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "" FORCE)
