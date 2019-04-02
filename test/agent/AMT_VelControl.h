//
// Created by Jens Klimke on 2019-03-23.
//

#ifndef SIMULATION_FRAMEWORK_AMT_VELCONTROL_H
#define SIMULATION_FRAMEWORK_AMT_VELCONTROL_H

#include <gtest/gtest.h>
#include <models/agent/AgentInjection.h>
#include <models/value/ValueOutOfTube.h>
#include "models/value/SignalCurve.h"
#include "../helpers/TestSimulation.h"


class AMT_VelControl :  public ::testing::Test, public AgentInjection {


protected:

    SignalCurve vDes{};


public:

    TestSimulation sim;

    AMT_VelControl() = default;

    bool step(double simTime) override {

        auto pred = vDes.where(_vehState->s);
        _drInject->state.conscious.vAntic = {pred.ds1, pred.v1};
        _drInject->state.conscious.vDes = vDes.previous(_vehState->s);

        return true;

    }

};

TEST_F(AMT_VelControl, VelocityLocalControl) {

    // set velocity profile
    auto dsStep = 350.0;
    vDes.define({  0.0, dsStep, INFINITY },
                { 27.0,   13.0,     13.0 });

    // create setup
    TestSimulation::SimSetup setup{};
    setup.stepSize = 0.01;
    setup.endTime = 40.0;
    setup.endDistance = dsStep;
    setup.logFile = "log/veh_driver_velocity.json";
    setup.injection = this;
    setup.agents.push_back({0.06, 0.0, vDes.previous(0.0), true, {nullptr, nullptr}});

    // create simulation
    sim.create(setup);

    // set parameters
    _drParam->cruise.thwMax = 0.0;

    // run simulation
    sim.loop.run();

    // check state
    EXPECT_NEAR(0.0, _vehState->a, 0.1);
    EXPECT_NEAR(_drParam->cruise.b, _drState->subConscious.aDes, 0.01);
    EXPECT_NEAR(vDes.previous(0.0), _vehState->v, 0.1);

}


TEST_F(AMT_VelControl, VelocityPredictiveControl) {

    // set velocity profile
    auto dsStep = 350.0;
    auto v0     =  27.0;
    auto v1     =  13.0;
    vDes.define({  0.0, dsStep},
                {   v0, v1});

    // create setup
    TestSimulation::SimSetup setup{};
    setup.stepSize    = 0.01;
    setup.endTime     = 40.0;
    setup.endDistance = dsStep + 50.0;
    setup.logFile = "log/veh_driver_velocity_pred.json";
    setup.injection = this;
    setup.agents.push_back({0.06, 0.0, vDes.previous(0.0), true, {nullptr, nullptr}});

    // create simulation
    sim.create(setup);

    // set parameters
    _drParam->cruise.thwMax = 15.0;
    _drParam->cruise.deltaPred = 4.0;


    auto dsPred = dsStep - v0 * _drParam->cruise.thwMax;

    ValueOutOfTube vob;
    vob.defineLower({0.0, dsPred}, {v0 - 0.1, v1 - 0.1});
    vob.defineUpper({0.0, dsStep}, {v0 + 0.1, v1 + 0.1});
    vob.setValues(&_vehState->s, &_vehState->v);

    sim.loop.addModel(&vob);
    sim.loop.addStopCondition(&vob);

    // run simulation
    sim.loop.run();

    // check if velocity is in tube
    ASSERT_NE(sim::IStopCondition::StopCode::OBJECTIVES_MISSED, vob.getCode());

    // check state
    EXPECT_NEAR(0.0, _vehState->a, 0.1);
    EXPECT_NEAR(0.0, _drState->subConscious.aDes, 0.1);
    EXPECT_NEAR(vDes.previous(_vehState->s), _vehState->v, 0.1);

}

#endif //SIMULATION_FRAMEWORK_AMT_VELCONTROL_H
