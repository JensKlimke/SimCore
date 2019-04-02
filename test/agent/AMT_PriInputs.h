//
// Created by Jens Klimke on 2019-03-23.
//

#ifndef SIMULATION_FRAMEWORK_AMT_PRIINPUTS_H
#define SIMULATION_FRAMEWORK_AMT_PRIINPUTS_H

#include <gtest/gtest.h>
#include <models/agent/AgentInjection.h>
#include "models/value/SignalCurve.h"
#include "../helpers/TestSimulation.h"


class AMT_PriInputs : public ::testing::Test, public AgentInjection {

protected:

    TestSimulation sim{};

    SignalCurve aDes{};
    SignalCurve dPsiDes{};


public:

    AMT_PriInputs() = default;
    ~AMT_PriInputs() override = default;


    bool step(double simTime) override {

        try {
            _drInject->state.subConscious.dPsiDes = dPsiDes.interpolate(simTime);
        } catch(...) {
            _drInject->state.subConscious.dPsiDes = 0.0;
        }


        try {

            // inject desired acceleration
            _drInject->state.subConscious.aDes = aDes.interpolate(simTime);

        } catch(...) {}


        // check if reaction is
        auto res = true;

        try {
            res &= aDes.hit(simTime, _drState->subConscious.aDes, 0.1);
        } catch(...) {}

        try {
            res &= dPsiDes.hit(simTime, _drState->subConscious.dPsiDes, 0.1);
        } catch(...) {}

        return res;

    }


};


TEST_F(AMT_PriInputs, sinInput) {

    // create state curve
    aDes.define({0.0, 5.0, 10.0, 15.0, 20.0, 30.0, 35.0, 40.0, 45.0, 50.0},
                {0.0, 0.0,  0.1,  0.1,  0.0,  0.0, -0.1, -0.1,  0.0,  0.0});

    // create
    dPsiDes.define({10.0, 12.0, 13.0, 15.0, 17.0, 18.0, 20.0},
                   { 0.0,  0.1,  0.1,  0.0, -0.1, -0.1,  0.0});

    // create setup
    TestSimulation::SimSetup setup{};
    setup.stepSize  = 0.01;
    setup.endTime   =  50.0;
    setup.logFile   = "log/veh_driver_sin.json";
    setup.injection = this;
    setup.agents.push_back({0.0, 0.0, 5.0, true, {nullptr, nullptr}});

    // create simulation
    sim.create(setup);
    sim.loop.run();

    // get vehicle state
    auto vehState = sim.agents[0].vehState;

    // check state
    EXPECT_NEAR(0.0, vehState->a, 1e-4);
    EXPECT_NEAR(0.0, vehState->dPsi, 1e-4);

}


#endif //SIMULATION_FRAMEWORK_AMT_PRIINPUTS_H
