set(SOURCE_FILES
        PrimaryController.cpp
        VehicleModel.cpp
        DriverController.cpp)

# define target
add_library(vehicle_model STATIC ${SOURCE_FILES})

# include directories
target_include_directories(vehicle_model PRIVATE ${PROJECT_SOURCE_DIR}/include)