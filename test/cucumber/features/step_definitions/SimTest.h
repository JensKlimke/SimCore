// Copyright (c) 2020 Jens Klimke.
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
// Created by Jens Klimke on 2020-09-30.
// Contributors:
//


#ifndef SIMCORE_SIM_TEST_H
#define SIMCORE_SIM_TEST_H

#include <simtraffic/TrafficSimulation.h>
#include <simtraffic/VehicleModelAdapter.h>

#ifndef TIME_STEP_SIZE
#define TIME_STEP_SIZE 0.01
#endif

auto const D = sim::traffic::VehicleModel::ShifterPosition::DRIVE;
auto const P = sim::traffic::VehicleModel::ShifterPosition::PARK;
auto const N = sim::traffic::VehicleModel::ShifterPosition::NEUTRAL;
auto const R = sim::traffic::VehicleModel::ShifterPosition::REVERSE;

struct LambdaExecuter : public sim::Model {

    double time = 0.0;
    double timeStepSize = 0.0;

    std::function<void()> lambda{};


};


struct SimTest : public sim::traffic::VehicleModelAdapter {


    sim::traffic::TrafficSimulation sim{};

    sim::traffic::VehicleModel::State *_state{};
    sim::traffic::VehicleModel::Input *_input{};
    sim::traffic::VehicleModel::Parameters *_params{};

    SimTest() {

        using namespace sim::traffic;

        // save state
        this->_state = &state;
        this->_input = &input;
        this->_params = &parameters;

        // set initial state
        state.xPosition = 0.0;
        state.yPosition = 0.0;
        state.velocity = 0.0;
        state.acceleration = 0.0;
        state.yawAngle = 0.0;
        state.yawRate = 0.0;

        // reset forces
        parameters.externalRelForce = 0.0;
        parameters.resistanceParameters[0] = 0.0;
        parameters.resistanceParameters[1] = 0.0;
        parameters.resistanceParameters[2] = 0.0;

        // set parameters parameters
        parameters.maxRelReverseDrivePower =  10.0;
        parameters.maxRelDrivePower        = 100.0;
        parameters.maxRelDriveTorque       =  10.0;
        parameters.maxRelBrakeTorque       =  10.0;
        parameters.maxCurvature            =   0.1;

        // reset inputs
        input.drive = 0.0;
        input.brake = 0.0;
        input.steering = 0.0;

    }

    void execute(double time) {

        // create simulation
        sim.create(time, TIME_STEP_SIZE, false);

        // add vehicle model
        this->setTimeStepSize(0.01);
        sim.addExternalComponent((sim::IComponent*) this);

        // run simulation
        sim.run();

    }

};

#endif // SIMCORE_SIM_TEST_H
