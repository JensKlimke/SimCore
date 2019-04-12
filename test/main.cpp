#include <gtest/gtest.h>

#include "SimTest.h"
#include "VehicleModelTest.h"
#include "AlgoTest.h"
#include "DMAlgoTest.h"
#include "agent_tests/AMT_PriInputs.h"
#include "agent_tests/AMT_VelControl.h"
#include "agent_tests/AMT_VelChoice.h"
#include "agent_tests/AMT_Stop.h"
#include "agent_tests/AMT_Follow.h"

int main(int argc, char **argv) {

    // run tests
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}