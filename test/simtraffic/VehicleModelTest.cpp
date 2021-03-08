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
// Created by Jens Klimke on 2021-03-06.
//

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <gtest/gtest.h>
#include <simtraffic/VehicleModel.h>

class VehicleModelTest : public ::testing::Test, public VehicleModel {

    void SetUp() override {

        parameters.maximumRelativePower = 1e2;
        parameters.maximumAcceleration = 10.0;
        parameters.maximumDeceleration = -10.0;
        parameters.maximumVelocity = 70.0;
        parameters.idleAcceleration = 0.2;

    }

};


TEST_F(VehicleModelTest, Standstill) {

    state.velocity = 0.0;

    input.pedal = 0.0; step(0.0);
    EXPECT_NEAR(0.2, state.acceleration, 1e-3);

    input.pedal = 1.0; step(0.0);
    EXPECT_NEAR(10.0, state.acceleration, 1e-3);

    input.pedal = -1.0; step(0.0);
    EXPECT_NEAR(-10.0, state.acceleration, 1e-3);

}


TEST_F(VehicleModelTest, IntermediateState) {

    state.velocity = 10.0;
    input.pedal = 1.0; step(0.0);
    EXPECT_NEAR(10.0, state.acceleration, 1e-3);

    state.velocity = 20.0;
    input.pedal = 1.0; step(0.0);
    EXPECT_NEAR(5.067, state.acceleration, 1e-3);

    state.velocity = 40.0;
    input.pedal = 1.0; step(0.0);
    EXPECT_NEAR(2.168, state.acceleration, 1e-3);

}


TEST_F(VehicleModelTest, MaximumSpeed) {

    // set to maximum velocity
    state.velocity = parameters.maximumVelocity;

    input.pedal = 1.0; step(0.0);
    EXPECT_NEAR(0.0, state.acceleration, 1e-3);

    input.pedal = -1.0; step(0.0);
    EXPECT_NEAR(-10.0, state.acceleration, 1e-3);

}


TEST_F(VehicleModelTest, Run) {

    // reset
    reset(0.0);

    // set input
    input.pedal = 1.0;

    // iterate over time
    for(unsigned int i = 0; i < 10000; ++i)
        step(0.1 * i);

    // check results
    EXPECT_NEAR(parameters.maximumVelocity, state.velocity, 1e-3);

    // set input
    input.pedal = -0.999;

    // iterate over time
    for(unsigned int i = 0; i < 10000; ++i)
        step(0.1 * i);

    // check results
    EXPECT_NEAR(0.0, state.velocity, 1e-3);

}


TEST_F(VehicleModelTest, ForewardsAndBackwards) {

    // check
    auto speeds = {0.0, 1.0, 5.0, 10.0, 20.0, 50.0, 70.0, 80.0};
    auto pedals = {-1.5, -1.0, -0.5, -0.1, 0.0, 0.1, 0.5, 1.0, 1.5};

    // iterate over velocities and pedals
    for(auto &v : speeds) {
        for (auto &p : pedals) {

            // calculate acceleration
            auto a = forwards(v, p);
            auto pc = backwards(v, a);

            // update original
            auto po = std::max(-1.0, std::min(1.0, pc));

            // calculate backwards
            EXPECT_NEAR(po, pc, 1e-3)  << "v=" << v << ", p=" << p;

        }
    }

}

#pragma clang diagnostic pop