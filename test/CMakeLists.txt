set(SOURCE_FILES
        SimTest.cpp
        SignalTest.cpp
        DataTest.cpp
        EnvironmentTest.cpp
        VehicleModelTest.cpp
        )

add_executable(SimTest ${SOURCE_FILES})
target_include_directories(SimTest PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(SimTest PRIVATE simcore)

add_gtest(SimTest)



add_executable(SimLiveTest TrafficSimulationTest.cpp)
target_include_directories(SimLiveTest PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(SimLiveTest PRIVATE simcore)

add_gtest(SimLiveTest)
