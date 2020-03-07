set(SOURCE_FILES
        SimTest.cpp
        SignalTest.cpp
        DataTest.cpp
        )

set(WEBSOCKET_SOURCE_FILES
        PublishTest.cpp
        )


# only test publisher, when websocket is active
if(BUILD_WEBSOCKET)
    add_executable(SimCoreTest ${SOURCE_FILES} ${WEBSOCKET_SOURCE_FILES})
    target_link_libraries(SimCoreTest PRIVATE simcore_publisher)
else()
    add_executable(SimCoreTest ${SOURCE_FILES})
endif()

add_gtest(SimCoreTest)
