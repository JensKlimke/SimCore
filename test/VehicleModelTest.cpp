//
// Created by klimke on 22.03.2019.
//

#include <gtest/gtest.h>
#include "TestSimulation.cpp"

class VehicleModelTest : public ::testing::Test, public TestSimulation {

public:


    VehicleModelTest() = default;

    ~VehicleModelTest() override = default;


};


TEST_F(VehicleModelTest, SteadyTurn) {

    // create setup
    SimSetup setup{};
    setup.endTime  = 200.0;
    setup.logFile  = "log/veh_steady.json";

    // create agent
    SimSetup::SimAgent ag{};
    ag.pedal =  1.0;
    ag.steer =  0.1;
    ag.v0    = 10.0;
    ag.control[0] = nullptr;
    ag.control[1] = nullptr;

    // add agent
    setup.agents.push_back(ag);

    // create simulation
    create(setup);

    // run simulation
    loop.run();

    // get vehicle state
    auto state = units[0]->vehState;

    EXPECT_NEAR(0.0, state->a, 0.01);
    // TODO: check if vehicle drives on steady curve (circle)

}


TEST_F(VehicleModelTest, Controlled) {

    // target value for acceleration
    auto aDes    = 0.0;
    auto dPsiDes = 0.3;

    // create setup
    SimSetup setup{};
    setup.endTime  =  20.0;
    setup.logFile  = "log/veh_controller.json";

    // create agent
    SimSetup::SimAgent ag{};
    ag.pedal =  1.0;
    ag.steer =  0.1;
    ag.v0    = 10.0;
    ag.control[0] = &aDes;
    ag.control[1] = &dPsiDes;

    // add agent
    setup.agents.push_back(ag);

    // create simulation
    create(setup);

    // run simulation
    loop.run();

    // get vehicle state
    auto state = units[0]->vehState;

    // check if acceleration is reached
    EXPECT_NEAR(aDes, state->a, 1e-8);
    EXPECT_NEAR(dPsiDes, state->dPsi, 1e-8);

}
