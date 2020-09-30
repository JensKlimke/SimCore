# create executable
add_executable(VirtualVehicleTest
        VehicleModelTest.cpp
        )

# include directories
target_include_directories(VirtualVehicleTest PRIVATE
        ${PROJECT_SOURCE_DIR}/src/include) # library include directory

# add test
add_gtest(VirtualVehicleTest)