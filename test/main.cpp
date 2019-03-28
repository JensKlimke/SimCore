#include <gtest/gtest.h>

#include "ModelTest.h"
#include "SimTest.h"
#include "VehicleModelTest.h"
#include "AgentModelTest.h"


int main(int argc, char **argv) {

    // run tests
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}