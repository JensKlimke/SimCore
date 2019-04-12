//
// Created by Jens Klimke on 2019-04-07.
//

#ifndef SIMULATION_FRAMEWORK_AMT_FOLLOW_H
#define SIMULATION_FRAMEWORK_AMT_FOLLOW_H

#include <gtest/gtest.h>
#include <models/agent/AgentInjection.h>
#include <models/value/SignalCurve.h>
#include "../helpers/TestSimulation.h"


class AMT_Follow : public ::testing::Test, public AgentInjection {

protected:

    TestSimulation sim{};

    // create setup
    TestSimulation::SimSetup setup{};

    // parameters
    double v0 = 20.0;
    double ds0;
    double vTar;
    double ds;

public:

    AMT_Follow() = default;
    ~AMT_Follow() override = default;


    void SetUp() override {

        setup.stepSize    = 0.01;
        setup.endTime     = 50.0;
        setup.endDistance = 400.0;
        setup.injection   = this;
        setup.agents.push_back({0.0, 0.0, v0, true, {nullptr, nullptr}});

    }


    bool step(double simTime) override {

        ds = ds0 + vTar * simTime - _vehState->s;

        _drInject->state.conscious.vDes = v0;
        _drInject->state.conscious.follow.ds = ds;
        _drInject->state.conscious.follow.value = vTar;

        return true;

    }


};


TEST_F(AMT_Follow, Follow) {

    // set parameters for test
    ds0  = 100.0;
    vTar = 10.0;

    // set log file
    setup.logFile = "log/veh_driver_follow.json";

    // create simulation
    sim.create(setup);
    sim.loop.run();

    // check values
    EXPECT_EQ(vTar, _vehState->v);
    EXPECT_EQ(10.0, ds);

}


#endif //SIMULATION_FRAMEWORK_AMT_FOLLOW_H
