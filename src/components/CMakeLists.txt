set(SOURCE_FILES
        VehicleModel.h VehicleModel.cpp
        timers/BasicTimer.h timers/BasicTimer.cpp
        timers/TimeIsUp.h timers/TimeIsUp.cpp
        timers/RealTimeTimer.h timers/RealTimeTimer.cpp
        data/DataManager.h data/DataManager.cpp
        data/JsonReporter.h
        data/TimeReporter.h
        value/ValueExceed.h
        value/ValueOutOfTube.h
        value/SignalTube.h
        environment/VirtualHorizon.h
        environment/Agent.h
        data/DataPublisher.h)

add_library(components STATIC ${SOURCE_FILES})
target_link_libraries(components odrfw curl)
target_include_directories(components PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/lib)