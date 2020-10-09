add_executable(StorageTest
        SignalTest.cpp
        DumpTest.cpp
        )

target_include_directories(StorageTest PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        )

target_link_libraries(StorageTest PUBLIC
        simcore_storage
        )

add_gtest(StorageTest)