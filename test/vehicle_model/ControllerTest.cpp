// Copyright (c) 2019 Institute for Automotive Engineering (ika), RWTH Aachen University. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// Created by Jens Klimke on 2019-03-22.
//

#include <gtest/gtest.h>
#include <TrafficSimulation/BasicSimulation.h>
#include <VehicleModel/VehicleModel.h>
#include <VehicleModel/DriverController.h>

class ControllerTest : public ::testing::Test, public VehicleModel, public BasicSimulation {

public:

    VehicleModel::Input      *input = nullptr;
    VehicleModel::Parameters *param = nullptr;
    VehicleModel::State      *state = nullptr;

    double accTarget = 0.0;
    double dPsiTarget = 0.0;
    double kappaTarget = 0.0;

    DriverController ctrl;

    ControllerTest() = default;
    ~ControllerTest() override = default;

    void SetUp() override {

        // create simulation
        create(20.0, 0.01, false);

        // set loop
        addComponent(this);

        // get pointers
        this->getInputs(&input);
        this->getParameters(&param);
        this->getStates(&state);

    }

    bool step(double simTime) override {

        auto dt = sinceLastTimeStep(simTime);

        // controller
        if(simTime > 0.0) {

            // get controller values
            double p = ctrl.getPedal(accTarget, dt, state->a, -9.81, 9.81);
            double s = ctrl.getSteeringByYawRate(dPsiTarget, dt, state->dPsi);

            // set vehicle inputs
            input->steer = s;
            input->pedal = p;

        } else {

            // set controllers to zero
            input->steer = 0.0;
            input->pedal = 0.0;

        }

        // vehicle step
        VehicleModel::step(simTime);

        return true;

    }

};


TEST_F(ControllerTest, ControlledYawRateDirectControllers) {

    // set target values
    accTarget = 0.0;
    dPsiTarget = 0.1;

    // set initial speed
    state->v = 10.0;

    // set pedal and steering value
    input->pedal = 0.0;
    input->steer = 0.1;
    input->slope = 0.0;

    // setup acceleration controller
    ctrl.pedal.setParameters(1.0, 0.0, 0.0);
    ctrl.steerYawRate.setParameters(1.0, 0.0, 0.0);

    // run simulation
    run();

    // this values need to fit
    EXPECT_NEAR(accTarget,  state->a,    1e-6);
    EXPECT_NEAR(dPsiTarget, state->dPsi, 1e-6);

}


TEST_F(ControllerTest, ControlledWithoutSim) {

    // set target values
    accTarget  = 0.0;
    dPsiTarget = 0.1;

    VehicleModel veh;
    DriverController ctrl;

    ctrl.pedal.setParameters(1.0, 0.0, 0.0);
    ctrl.steerYawRate.setParameters(1.0, 0.0, 0.0);

    veh.initialize(0.0);
    ctrl.pedal.initialize(0.0);
    ctrl.steerYawRate.initialize(0.0);

    double dt = 0.01;

    // get vehicle state
    VehicleModel::State *state = nullptr;
    veh.getStates(&state);

    // get vehicle input
    VehicleModel::Input *input = nullptr;
    veh.getInputs(&input);

    // set initial state
    state->delta =  0.1;
    state->v     = 10.0;

    for(unsigned int i = 0; i < 10.0 / dt; ++i) {

        double t = dt * i;

        input->pedal = ctrl.getPedal(accTarget, dt, state->a, -9.81, 9.81);
        input->steer = ctrl.getSteeringByYawRate(dPsiTarget, dt, state->dPsi);

        veh.step(t);

    }

    EXPECT_NEAR(accTarget,  state->a,    1e-6);
    EXPECT_NEAR(dPsiTarget, state->dPsi, 1e-6);

}


TEST_F(ControllerTest, ControlledWithoutSimCurvature) {

    // set target values
    accTarget   = 0.0;
    kappaTarget = 0.01;

    VehicleModel veh;
    DriverController ctrl;

    ctrl.pedal.setParameters(1.0, 0.0, 0.0);
    ctrl.steerCurv.setParameters(10.0, 0.0, 0.0);

    veh.initialize(0.0);
    ctrl.pedal.initialize(0.0);
    ctrl.steerCurv.initialize(0.0);

    double dt = 0.01;

    // get vehicle state
    VehicleModel::State *state = nullptr;
    veh.getStates(&state);

    // get vehicle input
    VehicleModel::Input *input = nullptr;
    veh.getInputs(&input);

    // set initial state
    state->delta =  0.0;
    state->v     = 10.0;

    for(unsigned int i = 0; i < 10.0 / dt; ++i) {

        double t = dt * i;

        input->pedal = ctrl.getPedal(accTarget, dt, state->a, -9.81, 9.81);
        input->steer = ctrl.getSteeringByCurvature(kappaTarget, dt, state->kappa);

        veh.step(t);

    }

    EXPECT_NEAR(accTarget,   state->a,     1e-6);
    EXPECT_NEAR(kappaTarget, state->kappa, 1e-6);

}


