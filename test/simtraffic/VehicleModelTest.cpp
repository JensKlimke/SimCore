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
#include <simtraffic/SetupVehicleModel.h>

class VehicleModelTest : public ::testing::Test, public simcore::traffic::VehicleModel {

protected:

    double maximumRelativePower = 90;
    double maximumAcceleration = 10.0;
    double maximumDeceleration = -10.0;
    double referenceVelocity = 12.0;
    double maximumVelocity = 70.0;
    double idleVelocity = 5.0;
    double idleAcceleration = 0.5;
    double minimumCurvature = 0.1;
    double maximumCurvature = 0.05;

    void SetUp() override {

        // setup longitudinal vehicle parameters
        simcore::traffic::setupVehicleByVelocityAndAccelerationStates(
                this,
                idleAcceleration,
                idleVelocity,
                referenceVelocity,
                maximumRelativePower,
                maximumVelocity,
                maximumAcceleration,
                maximumDeceleration
        );

        // setup lateral vehicle parameters
        simcore::traffic::setupVehicleByCurvature(
                this,
                minimumCurvature,
                maximumCurvature,
                maximumVelocity
        );

    }

    void initIdle() {

        // reset
        reset(0.0);

        // set input
        input.pedal = 0.0;
        input.steering = 0.0;

        // set state
        state.acceleration = 0.0;
        state.velocity = idleVelocity;
        state.curvature = 0.0;
        state.position = {0.0, 0.0, 0.0};
        state.yawAngle = 0.0;
        state.yawRate = 0.0;

    }


};


TEST_F(VehicleModelTest, SpecialCases) {

    // idle velocity: low = 0
    EXPECT_NEAR(0.0, parameters.low(idleVelocity + 1e-5), 1e-3);
    EXPECT_NEAR(0.0, parameters.low(idleVelocity),        1e-12);
    EXPECT_NEAR(0.0, parameters.low(idleVelocity - 1e-5), 1e-3);

    // max. velocity: high = 0
    EXPECT_NEAR(0.0, parameters.high(maximumVelocity), 1e-3);
    EXPECT_NEAR(0.0, parameters.low(idleVelocity), 1e-12);
    EXPECT_NEAR(idleAcceleration, parameters.low(0.0), 1e-12);
    EXPECT_NEAR(maximumAcceleration, parameters.high(0.0), 1e-12);

    // ref. velocity
    auto al = parameters.low(referenceVelocity);
    auto r = maximumRelativePower / referenceVelocity + al;
    EXPECT_NEAR(r, parameters.high(referenceVelocity),         1e-12);

    // check transition point for low
    auto l0 = parameters.low.front();
    auto l1 = parameters.low.back();
    EXPECT_NEAR(l0.polynomial(idleVelocity), l1.polynomial(idleVelocity), 1e-12);
    EXPECT_NEAR(l0.polynomial.derivative(idleVelocity), l1.polynomial.derivative(idleVelocity), 1e-12);
    EXPECT_NEAR(l0.polynomial.derivative(idleVelocity, 2), l1.polynomial.derivative(idleVelocity, 2), 1e-12);

    // check transition point for high
    auto h0 = parameters.high.front();
    auto h1 = parameters.high.back();
    EXPECT_NEAR(h0.polynomial(referenceVelocity), h1.polynomial(referenceVelocity), 1e-12);
    EXPECT_NEAR(h0.polynomial.derivative(referenceVelocity), h1.polynomial.derivative(referenceVelocity), 1e-12);
    EXPECT_NEAR(h0.polynomial.derivative(referenceVelocity, 2), h1.polynomial.derivative(referenceVelocity, 2), 1e-12);

}



TEST_F(VehicleModelTest, Standstill) {

    state.velocity = 0.0;

    input.pedal = 0.0; step(0.0);
    EXPECT_NEAR(idleAcceleration, state.acceleration, 1e-3);

    input.pedal = 1.0; step(0.0);
    EXPECT_NEAR(maximumAcceleration, state.acceleration, 1e-3);

    input.pedal = -1.0; step(0.0);
    EXPECT_NEAR(maximumDeceleration + idleAcceleration, state.acceleration, 1e-3);

}


TEST_F(VehicleModelTest, Idle) {

    state.velocity = idleVelocity;

    input.pedal = 0.0; step(0.0);
    EXPECT_NEAR(0.0, state.acceleration, 1e-3);

}


TEST_F(VehicleModelTest, MaximumSpeed) {

    // get deceleration at max. speed with pedal = 0
    double a = -maximumRelativePower / maximumVelocity;

    // set to maximum velocity
    state.velocity = maximumVelocity;

    input.pedal = 1.0; step(0.0);
    EXPECT_NEAR(0.0, state.acceleration, 1e-3);

    input.pedal = 0.0; step(0.0);
    EXPECT_NEAR(a, state.acceleration, 1e-3);

    input.pedal = -1.0; step(0.0);
    EXPECT_NEAR(maximumDeceleration + a, state.acceleration, 1e-3);

}


TEST_F(VehicleModelTest, LongForewardsAndBackwards) {

    // check
    auto speeds = {0.0, 1.0, 5.0, 10.0, 20.0, 50.0, 70.0, 80.0};
    auto pedals = {-1.5, -1.0, -0.5, -0.1, 0.0, 0.1, 0.5, 1.0, 1.5};

    // iterate over velocities and pedals
    for(auto &v : speeds) {
        for (auto &p : pedals) {

            // calculate acceleration
            auto a = longitudinalForwards(v, p);
            auto pc = longitudinalBackwards(v, a);

            // update original
            auto po = std::max(-1.0, std::min(1.0, pc));

            // calculate backwards
            EXPECT_NEAR(po, pc, 1e-3)  << "v=" << v << ", p=" << p;

        }
    }
}


TEST_F(VehicleModelTest, LatForewardsAndBackwards) {

    // check
    auto speeds = {0.0, 1.0, 5.0, 10.0, 20.0, 50.0, 70.0, 80.0};
    auto steering = {-1.5, -1.0, -0.5, -0.1, 0.0, 0.1, 0.5, 1.0, 1.5};

    // iterate over velocities and pedals
    for(auto &v : speeds) {
        for (auto &s : steering) {

            // calculate acceleration
            auto a = lateralForwards(v, s);
            auto pc = lateralBackwards(v, a);

            // update original
            auto po = std::max(-1.0, std::min(1.0, pc));

            // calculate backwards
            EXPECT_NEAR(po, pc, 1e-3)  << "v=" << v << ", s=" << s;

        }
    }
}


TEST_F(VehicleModelTest, Steering) {

    state.velocity = 0.0;

    input.steering = 0.0; step(0.0);
    EXPECT_NEAR( 0.0, state.curvature, 1e-5);

    input.steering = 0.5; step(0.0);
    EXPECT_NEAR( 0.05, state.curvature, 1e-5);

    input.steering = 1.0; step(0.0);
    EXPECT_NEAR( 0.1, state.curvature, 1e-5);

    input.steering = -0.5; step(0.0);
    EXPECT_NEAR(-0.05, state.curvature, 1e-5);

    input.steering = -1.0; step(0.0);
    EXPECT_NEAR(-0.1, state.curvature, 1e-5);

    EXPECT_NEAR( 0.0, state.yawRate, 1e-5);


    state.velocity = 70.0;

    input.steering = 0.0; step(0.0);
    EXPECT_NEAR( 0.0, state.curvature, 1e-5);

    input.steering = 0.5; step(0.0);
    EXPECT_NEAR( 0.025, state.curvature, 1e-5);

    input.steering = 1.0; step(0.0);
    EXPECT_NEAR( 0.05, state.curvature, 1e-5);

    input.steering = -0.5; step(0.0);
    EXPECT_NEAR(-0.025, state.curvature, 1e-5);

    input.steering = -1.0; step(0.0);
    EXPECT_NEAR(-0.05, state.curvature, 1e-5);

    EXPECT_NEAR(-3.5, state.yawRate, 1e-5);

}


TEST_F(VehicleModelTest, SteadyTurn) {

    state.velocity = 10.0;
    input.steering = 0.1;

    step(0.0);
    EXPECT_NEAR(0.0, state.yawAngle, 1e-6);

    step(0.1);
    EXPECT_NEAR(0.0098960, state.yawAngle, 1e-6);

    step(0.2);
    EXPECT_NEAR(0.0197901, state.yawAngle, 1e-6);

    step(0.3);
    EXPECT_NEAR(0.0296823, state.yawAngle, 1e-6);

}


TEST_F(VehicleModelTest, RunPosition) {

    // reset
    reset(0.0);

    // set input
    input.pedal = 1.0;
    input.steering = 0.0;

    // iterate over time
    for(unsigned int i = 0; i < 10000; ++i)
        step(0.1 * i);

    // check results
    EXPECT_NEAR(maximumVelocity, state.velocity, 1e-3);
    EXPECT_NEAR(0.0, state.yawRate, 1e-3);

    // set input
    input.pedal = -0.999;

    // iterate over time
    for(unsigned int i = 0; i < 10000; ++i)
        step(0.1 * i);

    // check results
    EXPECT_NEAR(0.0, state.velocity, 1e-3);
    EXPECT_NEAR(0.0, state.yawRate, 1e-3);

}


TEST_F(VehicleModelTest, RunPedal) {

    auto p = 1.0 / sqrt(2);

    // set idle speed
    initIdle();

    // set yaw angle
    state.yawAngle = 0.0;
    step(0.0); step(1.0);

    // check position
    EXPECT_NEAR(idleVelocity, state.position.x, 1e-3);
    EXPECT_NEAR(0.0, state.position.y, 1e-3);


    // set idle speed
    initIdle();

    // set yaw angle
    state.yawAngle = M_PI_4;
    step(0.0); step(1.0);

    // check position
    EXPECT_NEAR(p * idleVelocity, state.position.x, 1e-3);
    EXPECT_NEAR(p * idleVelocity, state.position.y, 1e-3);


    // set idle speed
    initIdle();

    // set yaw angle
    state.yawAngle = M_PI_2;
    step(0.0); step(1.0);

    // check position
    EXPECT_NEAR(0.0, state.position.x, 1e-3);
    EXPECT_NEAR(idleVelocity, state.position.y, 1e-3);


    // set idle speed
    initIdle();

    // set yaw angle
    state.yawAngle = M_PI_2 + M_PI_4;
    step(0.0); step(1.0);

    // check position
    EXPECT_NEAR(-p * idleVelocity, state.position.x, 1e-3);
    EXPECT_NEAR( p * idleVelocity, state.position.y, 1e-3);


    // set idle speed
    initIdle();

    // set yaw angle
    state.yawAngle = M_PI;
    step(0.0); step(1.0);

    // check position
    EXPECT_NEAR(-idleVelocity, state.position.x, 1e-3);
    EXPECT_NEAR(0.0, state.position.y, 1e-3);


    // set idle speed
    initIdle();

    // set yaw angle
    state.yawAngle = M_PI + M_PI_4;
    step(0.0); step(1.0);

    // check position
    EXPECT_NEAR(-p * idleVelocity, state.position.x, 1e-3);
    EXPECT_NEAR(-p * idleVelocity, state.position.y, 1e-3);


    // set idle speed
    initIdle();

    // set yaw angle
    state.yawAngle = M_PI + M_PI_2;
    step(0.0); step(1.0);

    // check position
    EXPECT_NEAR(0.0, state.position.x, 1e-3);
    EXPECT_NEAR(-idleVelocity, state.position.y, 1e-3);


    // set idle speed
    initIdle();

    // set yaw angle
    state.yawAngle = M_PI + M_PI_2 + M_PI_4;
    step(0.0); step(1.0);

    // check position
    EXPECT_NEAR( p * idleVelocity, state.position.x, 1e-3);
    EXPECT_NEAR(-p * idleVelocity, state.position.y, 1e-3);

}


TEST_F(VehicleModelTest, Eight) {

    // reset
    reset(0.0);

    // set initial state
    state.velocity = 0.0;
    state.acceleration = 0.0;
    state.yawAngle = 0.0;
    state.yawRate = 0.0;

    // iterate over time
    for(unsigned int i = 0; i < 100000; ++i) {

        // get time
        auto t = 0.01 * i;

        // set steering
        input.steering = sin(t * 0.01);

        // run step
        step(t);

    }

}

#pragma clang diagnostic pop