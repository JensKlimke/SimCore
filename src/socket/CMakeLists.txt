set(SOURCE_FILES
        DataPublisher.cpp
        WebSocket.cpp)

add_library(simcore_publisher STATIC ${SOURCE_FILES})
target_include_directories(simcore_publisher PRIVATE
        ${PROJECT_SOURCE_DIR}/include/simcore/socket
        ${Boost_INCLUDE_DIRS})
