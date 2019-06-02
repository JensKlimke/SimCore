set(SOURCE_FILES
        main.cpp
        SimTest.h
        SignalTest.h
        DataTest.h
        PluginTest.h
        EnvironmentTest.h
        PublishTest.h
        VehicleModelTest.h
        )

add_executable(SimTest ${SOURCE_FILES})
target_include_directories(SimTest PRIVATE ${PROJECT_SOURCE_DIR}/src ${})
target_link_libraries(SimTest PRIVATE core components)

add_gtest(SimTest)
