find_package(Protobuf REQUIRED)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS SimCore.proto)

add_library(simcore_storage STATIC
        Manager.cpp
        DataManager.cpp
        ${PROTO_SRCS} ${PROTO_HDRS}
        )

target_include_directories(simcore_storage
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${Protobuf_INCLUDE_DIRS}
        PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}
        ${PROJECT_SOURCE_DIR}/lib/json/include
        )

target_link_libraries(simcore_storage PUBLIC
        ${Protobuf_LIBRARIES}
        )