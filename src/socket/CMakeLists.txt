set(SOURCE_FILES
        UDPLogger.cpp
        UDPSocket.cpp
        DataPublisher.cpp
        WebSocket.cpp
        WebSocketLogger.cpp
        WebsocketTimer.cpp
        )

# find boost
find_package(Boost 1.40.0 REQUIRED system)

# create library
add_library(simcore_socket STATIC ${SOURCE_FILES})

# link libraries
target_include_directories(simcore_socket PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${Boost_INCLUDE_DIRS})

# installation
install(TARGETS simcore_socket DESTINATION lib)
