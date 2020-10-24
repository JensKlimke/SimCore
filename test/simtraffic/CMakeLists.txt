# create executable
add_executable(VirtualVehicleTest
        VehicleModelTest.cpp
        )

# include directories
target_include_directories(VirtualVehicleTest PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/lib/json/include
        )

target_link_libraries(VirtualVehicleTest PRIVATE
        simcore_storage
        )

# add test
add_gtest(VirtualVehicleTest)