set(SOURCE_FILES
        SimTest.cpp
        SignalTest.cpp
        DataTest.cpp
        PublishTest.cpp
        )

add_executable(SimTest ${SOURCE_FILES})
target_include_directories(SimTest PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(SimTest PRIVATE simcore)

add_gtest(SimTest)
