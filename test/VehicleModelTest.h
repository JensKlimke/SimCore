//
// Created by klimke on 22.03.2019.
//

#ifndef SIMULATION_FRAMEWORK_VEHICLEMODELTEST_H
#define SIMULATION_FRAMEWORK_VEHICLEMODELTEST_H

#include <gtest/gtest.h>
#include "TestSimulation.h"

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
    setup.agents.push_back({1.0, 0.1, 10.0, {nullptr, nullptr}});

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
    setup.agents.push_back({1.0, 0.1, 10.0, {&aDes, &dPsiDes}});

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



#endif //SIMULATION_FRAMEWORK_VEHICLEMODELTEST_H
