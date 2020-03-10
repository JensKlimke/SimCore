# define and create log dir
set(PROJECT_LOG_DIR ${PROJECT_SOURCE_DIR}/log)
file(MAKE_DIRECTORY ${PROJECT_LOG_DIR})
add_definitions(-DLOG_DIR=\"${PROJECT_LOG_DIR}\")

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