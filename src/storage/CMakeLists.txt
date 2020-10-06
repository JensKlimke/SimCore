find_package(Protobuf REQUIRED)

include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS SimCore.proto)

add_library(simcore_proto STATIC
        Manager.cpp
        ${PROTO_SRCS} ${PROTO_HDRS}
        )

target_include_directories(simcore_proto
        PRIVATE ${PROJECT_SOURCE_DIR}/include
        PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
        )

target_link_libraries(simcore_proto PUBLIC
        ${Protobuf_LIBRARIES}
        )