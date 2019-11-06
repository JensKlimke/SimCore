set(SOURCE_FILES
        DataPublisher.cpp
        WebSocket.cpp)

add_library(simcore_publisher STATIC ${SOURCE_FILES})
target_include_directories(simcore_publisher PRIVATE ${Boost_INCLUDE_DIRS})
