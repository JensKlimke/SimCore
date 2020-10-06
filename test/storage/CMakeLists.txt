add_executable(StorageTest
        ProtoTest.cpp
        SignalTest.cpp
        #DumpTest.cpp
        SimulationSignalTest.cpp
        )

target_include_directories(StorageTest PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        )

target_link_libraries(StorageTest PUBLIC
        simcore_storage
        )

add_gtest(StorageTest)