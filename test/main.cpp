#include "SimTest.h"
#include "SignalTest.h"
#include "ReporterTest.h"
#include "VirtualHorizonTest.h"

int main(int argc, char **argv) {

    // run tests
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}