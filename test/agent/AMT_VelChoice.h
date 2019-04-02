//
// Created by Jens Klimke on 2019-04-01.
//

#ifndef SIMULATION_FRAMEWORK_AMT_VELCHOICE_H
#define SIMULATION_FRAMEWORK_AMT_VELCHOICE_H

#include <gtest/gtest.h>
#include <models/agent/AgentInjection.h>
#include <models/value/ValueOutOfTube.h>
#include "models/value/SignalCurve.h"
#include "../helpers/TestSimulation.h"


class AMT_VelChoice :  public ::testing::Test, public AgentInjection {

protected:

    SignalCurve curvature{};
    SignalCurve speedLimit{};


public:

    TestSimulation sim;

    AMT_VelChoice() = default;

    bool step(double simTime) override {

        auto crv = curvature.where(_vehState->s);

        for(unsigned int i = 0; i < agmod::NOH; ++i) {

            auto ds = (double) i * (200.0 / agmod::NOH);
            auto s  = _vehState->s + ds;

            _drInject->input.environment.horizon[i].ds = ds;
            _drInject->input.environment.horizon[i].kappa = curvature.interpolate(s);

        }

        return true;

    }


};


TEST_F(AMT_VelChoice, VelocityLocalControl) {

    // set curvature profile and speed limit profile
    curvature.define({0.0, 100.0, 110.0, 190.0, 200.0, 1000.0}, {0.0, 0.0, 1.0 / 80.0, 1.0 / 80.0, 0.0, 0.0});
    speedLimit.define({300.0}, {100.0 / 3.6});

    auto vMaxDes = 130.0 / 3.6;

    // create setup
    TestSimulation::SimSetup setup{};
    setup.stepSize    = 0.01;
    setup.endTime     = 40.0;
    setup.endDistance = 400.0;
    setup.logFile     = "log/veh_driver_velocity_choice.json";
    setup.injection   = this;
    setup.agents.push_back({0.0, 0.0, vMaxDes, true, {nullptr, nullptr}});

    // create simulation
    sim.create(setup);

    // set parameters
    _drParam->cruise.vScale = 1.0;     // 100 kph
    _drParam->cruise.vMax   = vMaxDes; // 130 kph
    _drParam->cruise.ayMax  = 1.0;     //  80 kph

    // run simulation
    sim.loop.run();

}

#endif //SIMULATION_FRAMEWORK_AMT_VELCHOICE_H
