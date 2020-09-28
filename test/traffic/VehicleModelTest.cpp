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
// Created by Jens Klimke on 2020-08-04.
//

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <functional>
#include <gtest/gtest.h>
#include <simcore/traffic/VehicleModel.h>

class VehicleModelTest : public testing::Test, protected sim::traffic::VehicleModel {

protected:

    double preTime;
    double time;
    double counter;
    double dt = 0.01;

    std::function<void()> init{};
    std::function<void()> setStep{};
    std::function<void()> checkStep{};

    void run(double endTime) {

        // reset time
        time = 0.0;
        preTime = 0.0;
        counter = 0;

        // init
        if(init)
            init();

        // simple simulation loop
        while(time <= endTime) {

            // set step
            if(setStep)
                setStep();

            // run vehicle model step
            this->step(time - preTime);

            // check
            if(checkStep)
                checkStep();

            // update time
            preTime = time;
            time += dt;
            counter++;

        }

    }


    void resetFull() {

        // reset
        reset();

        // set initial state
        this->Unit::state.position.x = 0.0;
        this->Unit::state.position.y = 0.0;
        this->Unit::state.position.z = 0.0;
        this->Unit::state.velocity = 0.0;
        this->Unit::state.acceleration = 0.0;
        this->Unit::state.yawAngle = 0.0;
        this->Unit::state.yawAngle = 0.0;

        // reset inputs
        this->input.drive = 0.0;
        this->input.brake = 0.0;
        this->input.steering = 0.0;

        // switch to D
        this->setShifter(ShifterPosition::DRIVE);

    }


    void resetParameters() {

        // reset forces
        this->parameters.externalRelForce = 0.0;
        this->parameters.resistanceParameters[0] = 0.0;
        this->parameters.resistanceParameters[1] = 0.0;
        this->parameters.resistanceParameters[2] = 0.0;

    }


    void SetUp() override {

        // reset
        this->resetFull();
        this->resetParameters();

        // set parameters
        this->parameters.maxRelDrivePower  = 258.0;
        this->parameters.maxRelDriveTorque =  10.0;
        this->parameters.maxRelBrakeTorque =  10.0;

    }


    [[nodiscard]] bool isSimTime(double t) const {
        return std::abs(time - t) < EPS_SIM_TIME;
    }


    [[nodiscard]] bool isInterval(double t0, double t1) const {
        return (time > t0 - EPS_SIM_TIME && time < t1 - dt + EPS_SIM_TIME);
    }

};


TEST_F(VehicleModelTest, Process) {

    checkStep = [this]() {
         EXPECT_FALSE(this->_reset);
    };

    // reset and check
    this->reset();
    EXPECT_TRUE(this->_reset);

    // run
    this->run(10.0);
    EXPECT_EQ(1001, counter);

}


TEST_F(VehicleModelTest, MotionConstant) {


    // set checker
    checkStep = [this]() {

        // check state
        EXPECT_NEAR(10.0, this->Unit::state.velocity, 1e-3);
        EXPECT_NEAR(10.0 * time, this->state.distance, 1e-3);

    };

    // set dynamic state
    this->Unit::state.position.x = 100.0;
    this->Unit::state.position.y = 200.0;
    this->Unit::state.position.z = 300.0;
    this->Unit::state.yawAngle = M_PI / 8;
    this->Unit::state.velocity = 10.0;

    // run
    this->run(10.0);

    // check position
    EXPECT_NEAR(192.387953251, this->Unit::state.position.x, 1e-9);
    EXPECT_NEAR(238.268343236, this->Unit::state.position.y, 1e-9);
    EXPECT_DOUBLE_EQ(300.0, this->Unit::state.position.z);

}



TEST_F(VehicleModelTest, MotionAccelerated) {


    // set checker
    checkStep = [this]() {

        // calculate s and v
        double s = 0.05 * time * time + 10.0 * time;
        double v = 10.0 + 0.1 * time;

        // check
        EXPECT_NEAR(0.1, this->Unit::state.acceleration, 1e-6);
        EXPECT_NEAR(v, this->Unit::state.velocity, 1e-6);
        EXPECT_NEAR(s, this->state.distance, 1e-3);

    };

    // set external force and init state
    resetFull();
    this->Unit::state.velocity = 10.0;
    this->parameters.externalRelForce = 0.1;

    // run
    this->run(10.0);


}


TEST_F(VehicleModelTest, Resistances) {

    // reset
    resetFull();

    // set resistance
    this->parameters.resistanceParameters[0] = 1.0;
    this->parameters.resistanceParameters[1] = 0.0;
    this->parameters.resistanceParameters[2] = 0.0;

    // set dynamic state
    this->Unit::state.velocity = 10.0;

    // set checker
    checkStep = [this]() {

        // check until 10 s
        if(time >= 10.0 + EPS_SIM_TIME)
            return;

        // calculate s and v
        double s = -0.5 * time * time + 10.0 * time;
        double v = 10.0 - time;

        // check
        EXPECT_NEAR(-1.0, this->Unit::state.acceleration, 1e-6);
        EXPECT_NEAR(v, this->Unit::state.velocity, 1e-6);
        EXPECT_NEAR(s, this->state.distance, 1e-3);

    };

    // run, one more step than 10 s
    this->run(10.01);

    // check
    EXPECT_EQ(0.0, this->Unit::state.velocity);
    EXPECT_NEAR(50.0, this->state.distance, 1e-3);

}


TEST_F(VehicleModelTest, ResistancesBackwards) {

    // reset
    resetFull();
    this->setShifter(ShifterPosition::REVERSE);

    // set resistance
    this->parameters.resistanceParameters[0] = 1.0;
    this->parameters.resistanceParameters[1] = 0.0;
    this->parameters.resistanceParameters[2] = 0.0;

    // set dynamic state
    this->Unit::state.velocity = -10.0;

    // set checker
    checkStep = [this]() {

        // check until 10 s
        if(time >= 10.0 + EPS_SIM_TIME)
            return;

        // calculate s and v
        double s = -0.5 * time * time + 10.0 * time;
        double v = 10.0 - time;

        // check
        EXPECT_NEAR(1.0, this->Unit::state.acceleration, 1e-6);
        EXPECT_NEAR(-v, this->Unit::state.velocity, 1e-6);
        EXPECT_NEAR(-s, this->state.distance, 1e-3);

    };

    // run, one more step than 10 s
    this->run(10.01);

    // check
    EXPECT_EQ(0.0, this->Unit::state.velocity);
    EXPECT_NEAR(-50.0, this->state.distance, 1e-3);

}


TEST_F(VehicleModelTest, StartFromStandstill) {

    // set resistance
    this->parameters.resistanceParameters[0] = 0.1;   // general resistance
    this->parameters.resistanceParameters[1] = 0.01;  // rolling resistance
    this->parameters.resistanceParameters[2] = 0.001; // air drag

    // run with full drive power
    resetFull();
    this->input.drive = 1.0;
    this->run(1000.0);

    // check
    EXPECT_NEAR( 0.0, this->Unit::state.acceleration, 1e-6);
    EXPECT_NEAR(60.0, this->Unit::state.velocity, 1e-3);


    // run with 10% drive power
    resetFull();
    this->input.drive = 0.1;
    this->run(1000.0);

    // check
    EXPECT_NEAR( 0.0,   this->Unit::state.acceleration, 1e-6);
    EXPECT_NEAR(25.413, this->Unit::state.velocity, 1e-3);


    // run with 0% drive power
    resetFull();
    this->input.drive = 0.0;
    this->run(1000.0);

    // check
    EXPECT_NEAR(0.0, this->Unit::state.acceleration, 1e-6);
    EXPECT_NEAR(0.0, this->Unit::state.velocity, 1e-3);

}


TEST_F(VehicleModelTest, SlowDownToStop) {

    // set resistance
    this->parameters.resistanceParameters[0] = 1.0;   // general resistance
    this->parameters.resistanceParameters[1] = 0.01;  // rolling resistance
    this->parameters.resistanceParameters[2] = 0.001; // air drag

    // reset
    resetFull();

    // set initial conditions
    this->Unit::state.velocity = 5.0;
    this->input.drive = 0.01;

    // checker
    checkStep = [this]() {
        std::cout << this->Unit::state.velocity << std::endl;
    };

    // run
    this->run(10.0);

    // check
    EXPECT_NEAR(0.0, this->Unit::state.acceleration, 1e-6);
    EXPECT_NEAR(0.0, this->Unit::state.velocity, 1e-3);

}


TEST_F(VehicleModelTest, IndicatorStates) {

    this->indicateRight();
    EXPECT_EQ(IndicatorState::RIGHT, this->state.indicatorState);
    EXPECT_EQ(INFINITY, this->state.indicatorTimer);

    this->indicateLeft();
    EXPECT_EQ(IndicatorState::LEFT, this->state.indicatorState);
    EXPECT_EQ(INFINITY, this->state.indicatorTimer);

    this->hazard();
    EXPECT_EQ(IndicatorState::HAZARD, this->state.indicatorState);
    EXPECT_EQ(INFINITY, this->state.indicatorTimer);

    this->indicatorOff();
    EXPECT_EQ(IndicatorState::OFF, this->state.indicatorState);
    EXPECT_EQ(INFINITY, this->state.indicatorTimer);

    this->indicateRight(1.0);
    EXPECT_EQ(IndicatorState::RIGHT, this->state.indicatorState);
    EXPECT_EQ(1.0, this->state.indicatorTimer);

    this->indicateLeft(2.0);
    EXPECT_EQ(IndicatorState::LEFT, this->state.indicatorState);
    EXPECT_EQ(2.0, this->state.indicatorTimer);

    this->hazard();
    EXPECT_EQ(IndicatorState::HAZARD, this->state.indicatorState);
    EXPECT_EQ(INFINITY, this->state.indicatorTimer);

    this->indicateRight(3.0);
    EXPECT_EQ(IndicatorState::RIGHT, this->state.indicatorState);
    EXPECT_EQ(3.0, this->state.indicatorTimer);

    this->indicatorOff();
    EXPECT_EQ(IndicatorState::OFF, this->state.indicatorState);
    EXPECT_EQ(INFINITY, this->state.indicatorTimer);

    this->indicateRight(3.0);
    EXPECT_EQ(3.0, this->state.indicatorTimer);

    this->reset();
    EXPECT_EQ(INFINITY, this->state.indicatorTimer);

}


TEST_F(VehicleModelTest, IndicatorProzess) {

    // set indicators
    this->setStep = [this]() {

        if(isSimTime(5.0))
            this->indicateRight(3.0);
        else if(isSimTime(10.0))
            this->indicateLeft(2.0);
        else if(isSimTime(15.0))
            this->hazard();
        else if(isSimTime(19.0))
            this->indicatorOff();

    };

    // check indicators
    this->checkStep = [this]() {

        if(isInterval(0.0, 5.0))
            EXPECT_EQ(IndicatorState::OFF, this->state.indicatorState);
        else if(isInterval(5.0, 8.0))
            EXPECT_EQ(IndicatorState::RIGHT, this->state.indicatorState);
        else if(isInterval(8.0, 10.0))
            EXPECT_EQ(IndicatorState::OFF, this->state.indicatorState);
        else if(isInterval(10.0, 12.0))
            EXPECT_EQ(IndicatorState::LEFT, this->state.indicatorState);
        else if(isInterval(12.0, 15.0))
            EXPECT_EQ(IndicatorState::OFF, this->state.indicatorState);
        else if(isInterval(15.0, 19.0))
            EXPECT_EQ(IndicatorState::HAZARD, this->state.indicatorState);
        else if(isInterval(19.0, 21.0))
            EXPECT_EQ(IndicatorState::OFF, this->state.indicatorState);

    };

    // run simulation
    this->run(20.0);

}


TEST_F(VehicleModelTest, Shifter) {

    // switch to neutral
    setShifter(ShifterPosition::NEUTRAL);
    EXPECT_EQ(ShifterPosition::NEUTRAL, this->state.shifterPosition);

    // switch to drive
    setShifter(ShifterPosition::DRIVE);
    EXPECT_EQ(ShifterPosition::DRIVE, this->state.shifterPosition);

    // switch to reverse
    setShifter(ShifterPosition::REVERSE);
    EXPECT_EQ(ShifterPosition::REVERSE, this->state.shifterPosition);

    // switch to park
    setShifter(ShifterPosition::PARK);
    EXPECT_EQ(ShifterPosition::PARK, this->state.shifterPosition);

}

#pragma clang diagnostic pop