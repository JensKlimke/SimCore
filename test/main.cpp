#include <gtest/gtest.h>

#include "AlgoTest.h"
#include "SimTest.h"
#include "VehicleModelTest.h"
#include "agent/AMT_PriInputs.h"
#include "agent/AMT_VelControl.h"
#include "agent/AMT_VelChoice.h"
#include "agent/AMT_Stop.h"


int main(int argc, char **argv) {

    // run tests
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}