//
// Created by Jens Klimke on 2019-04-07.
//

#ifndef SIMULATION_FRAMEWORK_AMT_STOP_H
#define SIMULATION_FRAMEWORK_AMT_STOP_H

#include <gtest/gtest.h>
#include <models/agent/AgentInjection.h>
#include <models/value/SignalCurve.h>
#include "../helpers/TestSimulation.h"


class AMT_Stop : public ::testing::Test, public AgentInjection {

protected:

    TestSimulation sim{};

    SignalCurve stops{};


public:

    AMT_Stop() = default;
    ~AMT_Stop() override = default;


    bool step(double simTime) override {

        auto ds = stops.next(_vehState->s);
        _drInject->state.conscious.dsStop = ds;

        return true;

    }


};


TEST_F(AMT_Stop, Stop) {

    // create state curve
    stops.define({0.0, 100.0, 200.0, 500.0}, {0.0, 1.0, 2.0, 0.0});

    // create setup
    TestSimulation::SimSetup setup{};
    setup.stepSize    = 0.01;
    setup.endTime     = 50.0;
    setup.endDistance = 400.0;
    setup.logFile     = "log/veh_driver_stop.json";
    setup.injection   = this;
    setup.agents.push_back({0.0, 0.0, 5.0, true, {nullptr, nullptr}});

    // create simulation
    sim.create(setup);
    sim.loop.run();

}


#endif //SIMULATION_FRAMEWORK_AMT_STOP_H
