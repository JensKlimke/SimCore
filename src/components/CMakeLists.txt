set(SOURCE_FILES
        VehicleModel.h VehicleModel.cpp
        timers/BasicTimer.h timers/BasicTimer.cpp
        timers/TimeIsUp.h timers/TimeIsUp.cpp
        timers/RealTimeTimer.h timers/RealTimeTimer.cpp
        reporters/JsonReporter.h
        reporters/TimeReporter.h
        value/ValueExceed.h
        value/ValueOutOfTube.h
        value/SignalTube.h
        environment/VirtualHorizon.h
        )

add_library(components STATIC ${SOURCE_FILES})
target_link_libraries(components odrfw)
target_include_directories(components PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/lib)