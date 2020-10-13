# source files
set(SOURCE_FILES
        ExceptionTest.cpp
        StopConditionTest.cpp
        ValueCheckBasicTest.cpp
        ValueCheckTest.cpp
        LifeCycleTest.cpp
        LoggingTest.cpp
        StorageTest.cpp
        )

# executable
add_executable(SimCoreTest ${SOURCE_FILES})

# include directories
target_include_directories(SimCoreTest PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/lib/json/include
        )

target_link_libraries(SimCoreTest PRIVATE
        simcore_storage
        )

# add tests
add_gtest(SimCoreTest)
