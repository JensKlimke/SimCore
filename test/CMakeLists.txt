set(SOURCE_FILES
        SimTest.cpp
        SignalTest.cpp
        DataTest.cpp
        )


# only test publisher, when websocket is active
if(SIMCORE_ENABLE_WEBSOCKET)
    set(WEBSOCKET_SOURCE_FILES PublishTest.cpp)
else()
    set(WEBSOCKET_SOURCE_FILES "")
endif()


add_executable(SimCoreTest ${SOURCE_FILES} ${WEBSOCKET_SOURCE_FILES})
target_include_directories(SimCoreTest PRIVATE ${GTEST_INCLUDE_DIR})


# link publisher
if(SIMCORE_ENABLE_WEBSOCKET)
    target_link_libraries(SimCoreTest PRIVATE simcore_publisher)
endif()

add_gtest(SimCoreTest)
