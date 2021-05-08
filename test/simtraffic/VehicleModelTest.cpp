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
#include <simtraffic/SetupVehicleModel.h>

class VehicleModelTest : public ::testing::Test, public simtraffic::VehicleModel {

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
        simtraffic::setupVehicleByVelocityAndAccelerationStates(
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
        simtraffic::setupVehicleByCurvature(
                this,
                minimumCurvature,
                maximumCurvature,
                maximumVelocity
        );

    }

    void initIdle() {

        // reset
        initialize(0.0);

        // set input
        pedal = 0.0;
        steering = 0.0;

        // set state
        acceleration = 0.0;
        velocity = idleVelocity;
        curvature = 0.0;
        position = {0.0, 0.0};
        heading = {1.0, 0.0};
        yawRate = 0.0;

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

    velocity = 0.0;

    pedal = 0.0; step(0.0, 0.0);
    EXPECT_NEAR(idleAcceleration, acceleration, 1e-3);

    pedal = 1.0; step(0.0, 0.0);
    EXPECT_NEAR(maximumAcceleration, acceleration, 1e-3);

    pedal = -1.0; step(0.0, 0.0);
    EXPECT_NEAR(0.0, acceleration, 1e-3);

}



TEST_F(VehicleModelTest, Idle) {

    velocity = idleVelocity;

    pedal = 0.0; step(0.0, 0.0);
    EXPECT_NEAR(0.0, acceleration, 1e-3);

}


TEST_F(VehicleModelTest, MaximumSpeed) {

    // get deceleration at max. speed with pedal = 0
    double a = -maximumRelativePower / maximumVelocity;

    // set to maximum velocity
    velocity = maximumVelocity;

    pedal = 1.0; step(0.0, 0.0);
    EXPECT_NEAR(0.0, acceleration, 1e-3);

    pedal = 0.0; step(0.0, 0.0);
    EXPECT_NEAR(a, acceleration, 1e-3);

    pedal = -1.0; step(0.0, 0.0);
    EXPECT_NEAR(maximumDeceleration + a, acceleration, 1e-3);

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

    velocity = 0.0;

    steering = 0.0; step(0.0, 0.0);
    EXPECT_NEAR( 0.0, curvature, 1e-5);

    steering = 0.5; step(0.0, 0.0);
    EXPECT_NEAR( 0.05, curvature, 1e-5);

    steering = 1.0; step(0.0, 0.0);
    EXPECT_NEAR( 0.1, curvature, 1e-5);

    steering = -0.5; step(0.0, 0.0);
    EXPECT_NEAR(-0.05, curvature, 1e-5);

    steering = -1.0; step(0.0, 0.0);
    EXPECT_NEAR(-0.1, curvature, 1e-5);

    EXPECT_NEAR( 0.0, yawRate, 1e-5);


    velocity = 70.0;

    steering = 0.0; step(0.0, 0.0);
    EXPECT_NEAR( 0.0, curvature, 1e-5);

    steering = 0.5; step(0.0, 0.0);
    EXPECT_NEAR( 0.025, curvature, 1e-5);

    steering = 1.0; step(0.0, 0.0);
    EXPECT_NEAR( 0.05, curvature, 1e-5);

    steering = -0.5; step(0.0, 0.0);
    EXPECT_NEAR(-0.025, curvature, 1e-5);

    steering = -1.0; step(0.0, 0.0);
    EXPECT_NEAR(-0.05, curvature, 1e-5);

    EXPECT_NEAR(-3.5, yawRate, 1e-5);

}


TEST_F(VehicleModelTest, SteadyTurn) {

    velocity = 10.0;
    steering = 0.1;

    step(0.0, 0.0);
    EXPECT_NEAR(0.0, heading2angle(heading), 1e-6);

    step(0.1, 0.1);
    EXPECT_NEAR(0.0098960, heading2angle(heading), 1e-6);

    step(0.2, 0.1);
    EXPECT_NEAR(0.0197901, heading2angle(heading), 1e-6);

    step(0.3, 0.1);
    EXPECT_NEAR(0.0296823, heading2angle(heading), 1e-6);

}


TEST_F(VehicleModelTest, RunPosition) {

    // reset
    initialize(0.0);

    // set input
    pedal = 1.0;
    steering = 0.0;

    // iterate over time
    for(unsigned int i = 0; i < 10000; ++i)
        step(0.1 * i, 0.1);

    // check results
    EXPECT_NEAR(maximumVelocity, velocity, 1e-3);
    EXPECT_NEAR(0.0, yawRate, 1e-3);

    // set input
    pedal = -0.999;

    // iterate over time
    for(unsigned int i = 0; i < 10000; ++i)
        step(0.1 * i, 0.1);

    // check results
    EXPECT_NEAR(0.0, velocity, 1e-3);
    EXPECT_NEAR(0.0, yawRate, 1e-3);

}


TEST_F(VehicleModelTest, RunPedal) {

    auto p = 1.0 / sqrt(2);

    // set idle speed
    initIdle();

    // set yaw angle
    heading = {1.0, 0.0};
    step(0.0, 0.0);
    step(1.0, 1.0);

    // check position
    EXPECT_NEAR(idleVelocity, position.x, 1e-3);
    EXPECT_NEAR(0.0, position.y, 1e-3);


    // set idle speed
    initIdle();

    // set yaw angle
    heading = angle2heading(M_PI_4);
    step(0.0, 0.0);
    step(1.0, 1.0);

    // check position
    EXPECT_NEAR(p * idleVelocity, position.x, 1e-3);
    EXPECT_NEAR(p * idleVelocity, position.y, 1e-3);


    // set idle speed
    initIdle();

    // set yaw angle
    heading = angle2heading(M_PI_2);
    step(0.0, 0.0);
    step(1.0, 1.0);

    // check position
    EXPECT_NEAR(0.0, position.x, 1e-3);
    EXPECT_NEAR(idleVelocity, position.y, 1e-3);


    // set idle speed
    initIdle();

    // set yaw angle
    heading = angle2heading(M_PI_2 + M_PI_4);
    step(0.0, 0.0);
    step(1.0, 1.0);

    // check position
    EXPECT_NEAR(-p * idleVelocity, position.x, 1e-3);
    EXPECT_NEAR( p * idleVelocity, position.y, 1e-3);


    // set idle speed
    initIdle();

    // set yaw angle
    heading = angle2heading(M_PI);
    step(0.0, 0.0);
    step(1.0, 1.0);

    // check position
    EXPECT_NEAR(-idleVelocity, position.x, 1e-3);
    EXPECT_NEAR(0.0, position.y, 1e-3);


    // set idle speed
    initIdle();

    // set yaw angle
    heading = angle2heading(M_PI + M_PI_4);
    step(0.0, 0.0);
    step(1.0, 1.0);

    // check position
    EXPECT_NEAR(-p * idleVelocity, position.x, 1e-3);
    EXPECT_NEAR(-p * idleVelocity, position.y, 1e-3);


    // set idle speed
    initIdle();

    // set yaw angle
    heading = angle2heading(M_PI + M_PI_2);
    step(0.0, 0.0);
    step(1.0, 1.0);

    // check position
    EXPECT_NEAR(0.0, position.x, 1e-3);
    EXPECT_NEAR(-idleVelocity, position.y, 1e-3);


    // set idle speed
    initIdle();

    // set yaw angle
    heading = angle2heading(M_PI + M_PI_2 + M_PI_4);
    step(0.0, 0.0);
    step(1.0, 1.0);

    // check position
    EXPECT_NEAR( p * idleVelocity, position.x, 1e-3);
    EXPECT_NEAR(-p * idleVelocity, position.y, 1e-3);

}


TEST_F(VehicleModelTest, Eight) {

    // reset
    initialize(0.0);

    // set initial state
    velocity = 0.0;
    acceleration = 0.0;
    heading = {1.0, 0.0};
    yawRate = 0.0;

    // delta
    double dt = 0.01;

    // iterate over time
    for(unsigned int i = 0; i < 100000; ++i) {

        // get time
        auto t = dt * i;

        // set steering
        steering = sin(t * 0.01);

        // run step
        step(t, dt);

    }

}


TEST_F(VehicleModelTest, Distance) {

    velocity = 10.0;
    pedal = longitudinalBackwards(velocity, 0.0);


    step(0.0, 0.0);
    EXPECT_NEAR(0.0, distance, 1e-3);
    EXPECT_NEAR(0.0, acceleration, 1e-3);
    EXPECT_NEAR(10.0, velocity, 1e-3);

    step(0.1, 0.1);
    EXPECT_NEAR(1.0, distance, 1e-3);
    EXPECT_NEAR(0.0, acceleration, 1e-3);
    EXPECT_NEAR(10.0, velocity, 1e-3);

    auto d = getDistance();
    EXPECT_NEAR(1.0, d, 1e-3);

    step(0.2, 0.1);
    EXPECT_NEAR(2.0, distance, 1e-3);
    EXPECT_NEAR(0.0, acceleration, 1e-3);
    EXPECT_NEAR(10.0, velocity, 1e-3);

    step(0.3, 0.1);
    EXPECT_NEAR(3.0, distance, 1e-3);
    EXPECT_NEAR(0.0, acceleration, 1e-3);
    EXPECT_NEAR(10.0, velocity, 1e-3);

    d = getDistance();
    EXPECT_NEAR(2.0, d, 1e-3);

    step(0.4, 0.1);
    EXPECT_NEAR(4.0, distance, 1e-3);
    EXPECT_NEAR(0.0, acceleration, 1e-3);
    EXPECT_NEAR(10.0, velocity, 1e-3);

    d = getDistance();
    EXPECT_NEAR(1.0, d, 1e-3);


}


TEST_F(VehicleModelTest, Transpose) {

    // position ahead, vehicle in origin, heading east
    position = {0.0, 0.0};
    heading  = {1.0, 0.0};
    Vector2 absolut  = {1.0, 0.0};

    // transform (no rotation)
    auto v = toLocal(absolut);
    EXPECT_NEAR( 1.0, v.x, 1e-12);
    EXPECT_NEAR( 0.0, v.y, 1e-12);


    // moved position, vehicle moved, heading east
    position = { 1.0, 0.0};
    heading  = { 1.0, 0.0};
    absolut  = {11.0, 2.0};

    // transform (no rotation)
    v = toLocal(absolut);
    EXPECT_NEAR(10.0, v.x, 1e-12);
    EXPECT_NEAR( 2.0, v.y, 1e-12);


    // position ahead, vehicle in origin, heading north
    position = {0.0, 0.0};
    heading  = {0.0, 1.0};
    absolut  = {1.0, 0.0};

    // transform (no rotation)
    v = toLocal(absolut);
    EXPECT_NEAR( 0.0, v.x, 1e-12);
    EXPECT_NEAR(-1.0, v.y, 1e-12);


    // vehicle on position, heading north
    position = {1.0, 0.0};
    heading  = {0.0, 1.0};
    absolut  = {1.0, 0.0};

    // transform (no rotation)
    v = toLocal(absolut);
    EXPECT_NEAR( 0.0, v.x, 1e-12);
    EXPECT_NEAR( 0.0, v.y, 1e-12);


    // position ahead, vehicle in origin, heading east
    position = {0.0, 0.0};
    heading  = angle2heading(M_PI_4);
    absolut  = {1.0, 0.0};

    // transform (no rotation)
    v = toLocal(absolut);
    EXPECT_NEAR( 0.707, v.x, 1e-3);
    EXPECT_NEAR(-0.707, v.y, 1e-3);


    // position ahead, vehicle in origin, heading east
    position = {-0.5, -1.0};
    heading  = angle2heading(M_PI_4);
    absolut  = {0.5, 0.0};

    // transform (no rotation)
    v = toLocal(absolut);
    EXPECT_NEAR( 1.414, v.x, 1e-3);
    EXPECT_NEAR( 0.0,   v.y, 1e-3);


    // position ahead, vehicle in origin, heading east
    position = {0.0, 0.0};
    heading  = angle2heading(M_PI + M_PI_4);
    absolut  = {1.0, 0.0};

    // transform (no rotation)
    v = toLocal(absolut);
    EXPECT_NEAR(-0.707, v.x, 1e-3);
    EXPECT_NEAR( 0.707, v.y, 1e-3);


    // position ahead, vehicle in origin, heading east
    position = {102.836, -90.35};
    heading  = angle2heading(1.29568);
    absolut  = {-892.910, 2022.346};

    // transform (no rotation)
    v = toLocal(absolut);
    EXPECT_NEAR(1762.741, v.x, 1e-3);
    EXPECT_NEAR(1532.232, v.y, 1e-3);

}


TEST_F(VehicleModelTest, Heading) {

    position = {-1.0, 2.0};

    // vehicle heading east, angle = 0.0
    heading  = {1.0, 0.0};
    EXPECT_NEAR(0.0, toLocalAngle(0.0), 1e-12);

    // vehicle heading east, angle = pi/2
    heading  = {1.0, 0.0};
    EXPECT_NEAR(M_PI_2, toLocalAngle(M_PI_2), 1e-12);

    // vehicle heading east, angle = pi
    heading  = {1.0, 0.0};
    EXPECT_NEAR(M_PI, toLocalAngle(M_PI), 1e-12);

    // vehicle heading north, angle = 0.0
    heading  = {0.0, 1.0};
    EXPECT_NEAR(-M_PI_2, toLocalAngle(0.0), 1e-12);

    // vehicle heading north, angle = pi
    heading  = {0.0, 1.0};
    EXPECT_NEAR(M_PI_2, toLocalAngle(M_PI), 1e-12);

    // vehicle heading north, angle = 3/2 * pi
    heading  = {0.0, 1.0};
    EXPECT_NEAR(M_PI, toLocalAngle(3.0 * M_PI_2), 1e-12);

    // quadrants
    heading = angle2heading(1.0);
    EXPECT_NEAR(1.0, toLocalAngle(2.0), 1e-3);

    // quadrant: < pi/2
    heading = angle2heading(1.0);
    EXPECT_NEAR(2.0, toLocalAngle(3.0), 1e-3);

    // quadrant: < pi
    heading = angle2heading(1.0);
    EXPECT_NEAR(3.0, toLocalAngle(4.0), 1e-3);

    // quadrant: > pi
    heading = angle2heading(1.0);
    EXPECT_NEAR(4.0 - 2.0 * M_PI, toLocalAngle(5.0), 1e-3);

    // quadrant: > pi
    heading = angle2heading(1.0);
    EXPECT_NEAR(5.0 - 2.0 * M_PI, toLocalAngle(6.0), 1e-3);

    // quadrant: > 2 * pi
    heading = angle2heading(1.0);
    EXPECT_NEAR(7.0 - 2.0 * M_PI, toLocalAngle(8.0), 1e-3);

    // random
    heading = angle2heading(-2.3456789);
    EXPECT_NEAR(-0.136, toLocalAngle(-8.76543), 1e-3);

}

#pragma clang diagnostic pop