include(GoogleTest)

# Target must already exist
macro(add_gtest TESTNAME)

    target_link_libraries(${TESTNAME} PRIVATE GTest::gtest_main)

    gtest_discover_tests(${TESTNAME}
        TEST_PREFIX "${TESTNAME}."
        PROPERTIES FOLDER "Tests")

endmacro()
