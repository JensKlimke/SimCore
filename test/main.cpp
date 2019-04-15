#include <gtest/gtest.h>

#include "SimTest.h"
#include "AlgoTest.h"

int main(int argc, char **argv) {

    // run tests
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}