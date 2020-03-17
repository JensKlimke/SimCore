# define and create log dir
set(PROJECT_LOG_DIR ${PROJECT_SOURCE_DIR}/log)
file(MAKE_DIRECTORY ${PROJECT_LOG_DIR})
add_definitions(-DLOG_DIR=\"${PROJECT_LOG_DIR}\")

# source files
set(SOURCE_FILES
        SimTest.cpp
        SignalTest.cpp
        DataTest.cpp
        PlotTest.cpp
    )

# websocket
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
