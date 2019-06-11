set(SOURCE_FILES
        DataPublisher.cpp
        WebSocket.cpp)


# TODO: check if boost is installed in correct version
message(${Boost_INCLUDE_DIRS})

add_library(websocket STATIC ${SOURCE_FILES})
target_include_directories(websocket PRIVATE ${CMAKE_SOURCE_DIR}/src ${beast_INCLUDE_DIR})