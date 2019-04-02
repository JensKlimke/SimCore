set(SOURCE_FILES
        timers/BasicTimer.h timers/BasicTimer.cpp
        timers/TimeIsUp.h timers/TimeIsUp.cpp
        VehicleModel.h VehicleModel.cpp
        reporters/JsonReporter.h
        PrimaryController.h
        agent/ModelCollection.h agent/ModelCollection.cpp
        agent/AgentModel.cpp agent/AgentModel.h
        agent/AgentModelInterface.h
        agent/AgentModelDummyInjection.h
        agent/AgentModelAdapter.h
        agent/AgentInjection.h
        agent/AgentsLogger.h
        timers/RealTimeTimer.cpp timers/RealTimeTimer.h
        reporters/TimeReporter.h
        value/ValueExceed.h
        value/ValueOutOfTube.h value/SignalTube.h)

add_library(models ${SOURCE_FILES})
target_include_directories(models PUBLIC ../)
