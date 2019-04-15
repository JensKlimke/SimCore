set(SOURCE_FILES
        IModel.h
        Loop.cpp Loop.h
        Exceptions.h
        ITimer.h
        IStopCondition
        IParameterizable.h
        IStorable.h
        DataManager.h
        ISynchronized.h
        functions.h)


add_library(core ${SOURCE_FILES})
target_include_directories(core PUBLIC ../../lib)