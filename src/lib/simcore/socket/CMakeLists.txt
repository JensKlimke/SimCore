set(SOURCE_FILES
        DataPublisher.cpp
        WebSocket.cpp
        WebsocketTimer.cpp
        )

# find boost
find_package(Boost 1.40.0 REQUIRED system)

# create library
add_library(simcore_socket STATIC ${SOURCE_FILES})

# link libraries
target_include_directories(simcore_socket PRIVATE
        ${PROJECT_SOURCE_DIR}/src/include
        ${Boost_INCLUDE_DIRS})
