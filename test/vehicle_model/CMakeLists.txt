# create executable
add_executable(VehicleModelTest
        VehicleModelTest.cpp
        ControllerTest.cpp
        )

# link libs
target_link_libraries(VehicleModelTest PRIVATE
        vehicle_model
        traffic_simulation
        )

# include directories
target_include_directories(VehicleModelTest PRIVATE
        ${PROJECT_SOURCE_DIR}/app
        ${PROJECT_SOURCE_DIR}/include
        )

# add test
add_gtest(VehicleModelTest)