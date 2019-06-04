set(SOURCE_FILES
        IComponent.h
        Loop.h Loop.cpp
        Exceptions.h
        ITimer.h
        IStopCondition
        IParameterizable.h
        IStorable.h
        ISynchronized.h
        functions.h
        IPlugin.h Plugin.h
        Model.h
        )

add_library(core STATIC ${SOURCE_FILES})
