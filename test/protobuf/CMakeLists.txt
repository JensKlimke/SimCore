find_package(Protobuf REQUIRED)

include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS SimCore.proto)

add_executable(ProtoTest
        ProtoTest.cpp
        ${PROTO_SRCS} ${PROTO_HDRS}
        )

target_link_libraries(ProtoTest PUBLIC
        ${Protobuf_LIBRARIES}
        )

add_gtest(ProtoTest)