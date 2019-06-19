set(SOURCE_FILES
        DataPublisher.cpp
        WebSocket.cpp)

add_library(websocket STATIC ${SOURCE_FILES})
target_include_directories(websocket PRIVATE ${PROJECT_SOURCE_DIR}/src ${Boost_INCLUDE_DIRS})