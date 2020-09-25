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
#include "PIDController.h"


class BlockTest : public ::testing::Test, public PIDController {

};


TEST_F(BlockTest, RunBlock) {

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
    double endTime = 100.0;
    double deltaTime = 0.1;

    // initialize model
    initialize(0.0);

    // main loop
    while(time <= endTime + EPS_TIME) {

        // set input
        input.targetVelocity = vTarget;

        // execute model
        step(time, deltaTime);

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