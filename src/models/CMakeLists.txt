set(SOURCE_FILES
        BasicTimer.h BasicTimer.cpp
        TimeIsUp.h TimeIsUp.cpp
        VehicleModel.h VehicleModel.cpp
        JsonReporter.h
        PrimaryController.h
        agent/ModelCollection.h agent/ModelCollection.cpp
        agent/AgentModel.cpp agent/AgentModel.h
        agent/AgentModelInterface.h
        agent/AgentModelDummyInjection.h
        agent/AgentModelAdapter.h
        agent/AgentInjection.h RealTimeTimer.cpp RealTimeTimer.h TimeReporter.h agent/AgentsLogger.h)

add_library(models ${SOURCE_FILES})
target_include_directories(models PUBLIC ../)
