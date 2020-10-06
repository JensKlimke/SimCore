add_executable(ProtoTest
        ProtoTest.cpp
        )

target_include_directories(ProtoTest PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        )

target_link_libraries(ProtoTest PUBLIC
        simcore_proto
        )

add_gtest(ProtoTest)