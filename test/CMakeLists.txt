set(SOURCE_FILES
        SimTest.cpp
        SignalTest.cpp
        DataTest.cpp
        PublishTest.cpp
        )

add_executable(SimCoreTest ${SOURCE_FILES})
target_include_directories(SimCoreTest PRIVATE ${GTEST_INCLUDE_DIR})
target_link_libraries(SimCoreTest PRIVATE simcore_publisher)

add_gtest(SimCoreTest)
