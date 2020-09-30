# boost
find_package(Boost 1.40.0 REQUIRED system)

# source files
set(SOURCE_FILES
        RemoteLoggingTest.cpp
    )

# executable
add_executable(ServerTest ${SOURCE_FILES})

# include directories
target_include_directories(ServerTest PRIVATE
        ${PROJECT_SOURCE_DIR}/src/simcore
        ${PROJECT_SOURCE_DIR}/src/include
        ${Boost_INCLUDE_DIRS}
        )

# link socket library
target_link_libraries(ServerTest PRIVATE
        simcore_socket
        )

# add tests
add_gtest(ServerTest)
