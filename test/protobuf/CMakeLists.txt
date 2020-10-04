add_executable(ProtoTest
        ProtoTest.cpp
        )

target_include_directories(ProtoTest PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        )

target_link_libraries(ProtoTest PUBLIC
        simcore_dump
        )

add_gtest(ProtoTest)