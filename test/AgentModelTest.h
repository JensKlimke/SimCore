//
// Created by Jens Klimke on 2019-03-23.
//

#ifndef SIMULATION_FRAMEWORK_AGENTMODELTEST_H
#define SIMULATION_FRAMEWORK_AGENTMODELTEST_H

#include <gtest/gtest.h>
#include <models/agent/AgentInjection.h>


class SinInjection : public AgentInjection {

public:

    bool step(double simTime) override {

        // inject desired yaw rate
        if(simTime >= 10.0 && simTime <= 20.0)
            _drInject->state.subConscious.dPsiDes = 0.1 * sin(2.0 * M_PI * (simTime - 10.0) / 10.0);
        else
            _drInject->state.subConscious.dPsiDes = 0.0;

        // inject desired velocity
        if(simTime >= 5.0 && simTime <= 25.0)
            _drInject->state.conscious.vDes = 10.0;
        else
            _drInject->state.conscious.vDes = 5.0;

        return true;

    }

};


class VelocityInjection : public AgentInjection {


public:

    double v0{};
    double vStep{};
    double s0{};

    bool step(double simTime) override {

        // get vehicle position
        double ds = s0 - _vehState->xy[0];

        // add speed change to conscious state
        _drInject->state.conscious.vAntic.ds = ds;
        _drInject->state.conscious.vAntic.value = vStep;

        // set desired velocity
        _drInject->state.conscious.vDes = ds < 0.0 ? vStep : v0;

        return true;

    }

};


class AgentModelTest : public ::testing::Test, public TestSimulation {


public:

    AgentModelTest() = default;

    ~AgentModelTest() override = default;

};


TEST_F(AgentModelTest, sinInput) {

    // create injection
    SinInjection inj{};

    // create setup
    SimSetup setup{};
    setup.stepSize =   0.01;
    setup.endTime =  50.0;
    setup.logFile = "log/veh_driver_sin.json";
    setup.injection = &inj;
    setup.agents.push_back({0.0, 0.0, 5.0, true, {nullptr, nullptr}});

    // create simulation
    create(setup);

    // run simulation
    sim.run();

    // get vehicle state
    auto vehState = agents[0].vehState;

    // check state
    EXPECT_NEAR(0.0, vehState->a, 1e-4);
    EXPECT_NEAR(5.0, vehState->v, 1e-4);

}


TEST_F(AgentModelTest, velInput) {

    // create injection
    VelocityInjection inj{};

    // set speed steps
    inj.v0    = 20.0;
    inj.vStep = 10.0;
    inj.s0    = 400.0;

    // create setup
    SimSetup setup{};
    setup.stepSize = 0.01;
    setup.endTime = 21.0;
    setup.logFile = "log/veh_driver_velocity.json";
    setup.injection = &inj;
    setup.realTime = true;
    setup.timeReport = true;
    setup.liveLog = true;
    setup.agents.push_back({0.0, 0.0, inj.v0, true, {nullptr, nullptr}});

    // create simulation
    create(setup);

    // run simulation
    sim.run();

    // get vehicle state
    auto vehState = agents[0].vehState;

    // check state
    EXPECT_NEAR(0.0, vehState->a, 1e-4);
    EXPECT_NEAR(inj.vStep, vehState->v, 1e-4);

}

#endif //SIMULATION_FRAMEWORK_AGENTMODELTEST_H
