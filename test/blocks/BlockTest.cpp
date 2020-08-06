//
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
// Created by Jens Klimke on 2020-08-06.
//

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <map>
#include <gtest/gtest.h>
#include <simcore/blocks/Block.h>
#include <simcore/blocks/Signal.h>

typedef double sim_time_t;

#ifndef EPS_TIME
#define EPS_TIME 1e-3
#endif

#ifndef DEBUG
#define DEBUG
#endif

#ifdef DEBUG
#define SIGNAL(owner, name) Signal<double> name{#owner, #name}
#else
#define SIGNAL(owner, name) double name
#endif


struct ParamPID {
    double kP;
    double kI;
    double kD;
};

struct Init {
    double delay = 0.0;
    struct {
        double sum = 0.0;
        double error = 0.0;
    } pid;
    double integral = 0.0;
    double acceleration = 0.0;
    double velocity = 0.0;
};

static Init init;

struct Input {
    double targetVelocity;
};

struct Output {
    double velocity;
    double acceleration;
};

struct State {
    SIGNAL(block.test, target);
    SIGNAL(block.test, delay);
    SIGNAL(block.test, error);
    SIGNAL(block.test, control);
    SIGNAL(block.test, system);
};

struct MemoryDelay {
    double value;
};

struct MemoryIntegral {
    double value;
};

struct MemoryPID {
    double sum;
    double err;
};


class BlockTest : public ::testing::Test, public sim::blocks::Block {

protected:

    sim_time_t _time;

    struct {
        ParamPID pid;
    } parameters;

    Input input;
    State state;
    Output output;

    struct {
        MemoryDelay delay;
        MemoryPID pid;
        MemoryIntegral integral;
    } memory;

public:


    void initialize(sim_time_t t) {

        // set time
        _time = t;

        // reset PID
        memory.pid.sum = init.pid.sum;
        memory.pid.err = init.pid.error;

        // reset Delay
        memory.delay.value = init.delay;

        // reset integral
        memory.integral.value = init.integral;

        // reset outputs
        output.acceleration = init.acceleration;
        output.velocity = init.velocity;

    }

    void exec(sim_time_t t) {

        // time step size
        sim_time_t dt = t - _time;

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

    void terminate(sim_time_t t) {

    }

};


TEST_F(BlockTest, ReportTime) {

    // model setup
    double vTarget = 10.0;
    double v = 0.0;
    double a = 0.0;

    // set parameters
    parameters.pid.kP = 0.1;
    parameters.pid.kI = 0.0;
    parameters.pid.kD = 0.0;

    // simulation setup
    double time = 0.0;
    double endTime = 10000000.0;
    double deltaTime = 0.1;

    // initialize model
    initialize(0.0);

    // main loop
    while(time <= endTime + EPS_TIME) {

        // set input
        input.targetVelocity = vTarget;

        // execute model
        exec(time);

        // set output
        v = output.velocity;
        a = output.acceleration;

        // update time
        time += deltaTime;

    }

    // terminate model
    terminate(time);

    // check
    EXPECT_NEAR(vTarget, v, 1e-3);
    EXPECT_NEAR(0.0, a, 1e-3);

}


#pragma clang diagnostic pop