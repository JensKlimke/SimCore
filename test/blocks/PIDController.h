// Copyright (c) 2020 Jens Klimke (jens.klimke@rwth-aachen.de). All rights reserved.
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
// Created by Jens Klimke on 2020-08-21.
//


#ifndef SIMCORE_PID_CONTROLLER_H
#define SIMCORE_PID_CONTROLLER_H

#include <simcore/Model.h>
#include "Global.h"
#include "PIDController_Interface.h"


class PIDController : public sim::Model {


protected:

    Input input;
    State state;
    Output output;

    Parameters parameters;
    Memory memory;

    void initialize(sim_time_t t) override {

        // reset PID
        memory.pid.sum = Init.pid.sum;
        memory.pid.err = Init.pid.error;

        // reset Delay
        memory.delay.value = Init.delay;

        // reset integral
        memory.integral.value = Init.integral;

        // reset outputs
        output.acceleration = Init.acceleration;
        output.velocity = Init.velocity;

    }

    void step(sim_time_t t, sim_time_t dt) override {

        // read inputs
        state.target = input.targetVelocity;

        // read internals
        state.delay = memory.delay.value;

        // calculate error
        state.error = state.target - state.delay;

        // calculate PID controller
        state.control =
                parameters.pid.kP * state.error
                + parameters.pid.kI * memory.pid.sum
                + parameters.pid.kD * ((dt > EPS_TIME) ? (state.error - memory.pid.err) : 0.0);

        // write memory for PID controller
        memory.pid.sum += state.error * dt;
        memory.pid.err = state.error;

        // calculate integral
        memory.integral.value += state.control * dt;
        state.system = memory.integral.value;

        // write memory for delay
        memory.delay.value = state.system;

        // write outputs
        output.velocity = state.system;
        output.acceleration = state.control;

    }

    void terminate(sim_time_t t) override {

    }

};


#endif // SIMCORE_PID_CONTROLLER_H
