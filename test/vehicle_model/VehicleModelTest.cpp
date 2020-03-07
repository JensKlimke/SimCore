// Copyright (c) 2019-2020 Jens Klimke <jens.klimke@rwth-aachen.de>
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
#include <VehicleModel/DriverController.h>
#include <VehicleModel/VehicleModel.h>

class VehicleModelTest : public ::testing::Test, public BasicSimulation, public VehicleModel {

public:

    VehicleModel::Input *input = nullptr;
    VehicleModel::Parameters *param = nullptr;
    VehicleModel::State *state = nullptr;

    VehicleModelTest() = default;
    ~VehicleModelTest() override = default;


    void SetUp() override {}


    void create() {

        // create simulation
        BasicSimulation::create(1000.0, 0.01, false);

        // add vehicle as component
        addComponent(this);

        // get pointers
        this->getInputs(&input);
        this->getParameters(&param);
        this->getStates(&state);

        // set vehicle parameters
        param->steerTransmission  = 0.5;
        param->wheelBase          = 3.0;
        param->cwA                = 0.6;
        param->mass               = 1.5e3;
        param->powerMax           = 1.0e5;
        param->forceMax           = 1.5e4;
        param->idle               = 0.05;
        param->rollCoefficient[0] = 4.0 * 9.91e-3;
        param->rollCoefficient[1] = 4.0 * 1.95e-5;
        param->rollCoefficient[2] = 4.0 * 1.76e-9;
        param->size[0]            = 5.0;
        param->size[0]            = 2.2;
        param->driverPos[0]       = 0.5;
        param->driverPos[1]       = 0.5;

    }


};





TEST_F(VehicleModelTest, MaxSpeed) {

    create();

    // set pedal and steering value
    input->pedal = 1.0;
    input->steer = 0.0;
    input->slope = 0.0;

    // set parameters
    param->steerTransmission  = 1.0;
    param->wheelBase          = 3.0;
    param->cwA                = 1.0;
    param->mass               = 1.0e3;
    param->powerMax           = 1.0e5;
    param->powerMax           = 1.0e4;
    param->rollCoefficient[0] = 0.0;
    param->rollCoefficient[1] = 0.0;
    param->rollCoefficient[2] = 0.0;

    // calculates KPIs of the vehicle model
    double cwa = 0.5 * 1.2041 * param->cwA;
    double rhs = std::pow(param->powerMax / cwa, 1.0 / 3.0);

    // run simulation
    run();

    EXPECT_NEAR(0.0, state->a, 1e-4);
    EXPECT_NEAR(rhs, state->v, 1e-4);

}


TEST_F(VehicleModelTest, MaxSpeedWithStdParams) {

    create();

    // set pedal and steering value
    input->pedal = 1.0;
    input->steer = 0.0;
    input->slope = 0.1;

    // run simulation
    run();

    EXPECT_NEAR(    0.0,       state->a,  1e-3);
    EXPECT_NEAR(   38.1739642, state->v,  1e-7);
    EXPECT_NEAR(    0.3817396, state->ds, 1e-7);
    EXPECT_NEAR(37822.5064121, state->s,  1e-7);

}


TEST_F(VehicleModelTest, IdleSpeedWithStdParams) {

    create();

    // set pedal and steering value
    input->pedal = 0.0;
    input->steer = 0.0;

    // run simulation
    run();

    EXPECT_NEAR(    0.0,       state->a,  1e-3);
    EXPECT_NEAR(   8.11176029, state->v,  1e-7);
    EXPECT_NEAR(    0.0811176, state->ds, 1e-7);
    EXPECT_NEAR(7789.26159312, state->s,  1e-7);

}


TEST_F(VehicleModelTest, SteadyTurn) {

    create();

    // set pedal and steering value
    input->pedal = 0.1;
    input->steer = 0.1;
    input->slope = 0.0;

    // run simulation
    run();

    EXPECT_NEAR(0.0,     state->a, 1e-4);
    EXPECT_NEAR(19.5095, state->v, 1e-4);

    EXPECT_NEAR(  0.3251588, state->dPsi, 1e-7);
    EXPECT_NEAR(318.9511627, state->psi,  1e-7);

}


TEST_F(VehicleModelTest, ControlledYawRate) {

    create();

    double accTarget = 0.0;
    double dPsiTarget = 0.5;

    // set initial speed
    state->v = 10.0;

    // set pedal and steering value
    input->pedal = 0.1;
    input->steer = 0.1;
    input->slope = 0.0;

    // create controllers
    PrimaryController accCtrl;
    PrimaryController steerCtrl;

    // setup acceleration controller
    accCtrl.setParameters(1.0, 0.0, 0.0);
    accCtrl.setVariables(&state->a, &accTarget, &input->pedal);

    // setup steering controller
    steerCtrl.setParameters(1.0, 0.0, 0.0);
    steerCtrl.setVariables(&state->dPsi, &dPsiTarget, &input->steer);

    // add components
    addComponent(&accCtrl);
    addComponent(&steerCtrl);

    // run simulation
    run();

    // this values need to fit
    EXPECT_NEAR(0.0,        state->a,    1e-7);
    EXPECT_NEAR(dPsiTarget, state->dPsi, 1e-7);

}


TEST_F(VehicleModelTest, ControlledCurvature) {

    create();

    double accTarget = 0.0;
    double kappaTarget = 0.1;

    // set initial speed
    state->v = 10.0;

    // set pedal and steering value
    input->pedal = 0.1;
    input->steer = 0.1;
    input->slope = 0.0;

    // create controllers
    PrimaryController accCtrl;
    PrimaryController steerCtrl;

    // setup acceleration controller
    accCtrl.setParameters(1.0, 0.0, 0.0);
    accCtrl.setVariables(&state->a, &accTarget, &input->pedal);

    // setup steering controller
    steerCtrl.setParameters(10.0, 0.0, 0.0);
    steerCtrl.setVariables(&state->kappa, &kappaTarget, &input->steer);

    // add components
    addComponent(&accCtrl);
    addComponent(&steerCtrl);

    // run simulation
    run();

    // this values need to fit
    EXPECT_NEAR(0.0,         state->a,     1e-7);
    EXPECT_NEAR(kappaTarget, state->kappa, 1e-7);

}