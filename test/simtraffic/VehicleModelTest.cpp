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
#include <simtraffic/VehicleModel.h>

class VehicleModelTest : public testing::Test, protected sim::traffic::VehicleModel {

protected:

    double preTime{};
    double time{};
    double counter{};
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
            step(time - preTime);

            // check
            if(checkStep)
                checkStep();

            // update time
            preTime = time;
            time += dt;

            // increment counter
            counter++;

        }

    }


    void resetFull() {

        // reset
        reset();

        // set initial state
        state.xPosition = 0.0;
        state.yPosition = 0.0;
        state.velocity = 0.0;
        state.acceleration = 0.0;
        state.yawAngle = 0.0;
        state.yawRate = 0.0;

        // reset inputs
        input.drive = 0.0;
        input.brake = 0.0;
        input.steering = 0.0;

        // switch to D
        setShifter(ShifterPosition::DRIVE);

    }


    void resetParameters() {

        // reset forces
        parameters.externalRelForce = 0.0;
        parameters.resistanceParameters[0] = 0.0;
        parameters.resistanceParameters[1] = 0.0;
        parameters.resistanceParameters[2] = 0.0;

    }


    void SetUp() override {

        // reset
        resetFull();
        resetParameters();

        // set parameters
        parameters.maxRelReverseDrivePower =  10.0;
        parameters.maxRelDrivePower        = 100.0;
        parameters.maxRelDriveTorque       =  10.0;
        parameters.maxRelBrakeTorque       =  10.0;
        parameters.maxCurvature            =   0.1;

    }


    [[nodiscard]] bool isSimTime(double t) const {
        return std::abs(time - t) < EPS_SIM_TIME;
    }


    [[nodiscard]] bool isInterval(double t0, double t1) const {
        return (time > t0 - EPS_SIM_TIME && time < t1 - dt + EPS_SIM_TIME);
    }

};


TEST_F(VehicleModelTest, SimProcess) {

    checkStep = [this]() {
         EXPECT_FALSE(_reset);
    };

    // reset and check
    reset();
    EXPECT_TRUE(_reset);

    // run
    run(10.0);
    EXPECT_EQ(1001, counter);

}


TEST_F(VehicleModelTest, ConstantLongMotion) {


    // set checker
    checkStep = [this]() {

        // check state
        EXPECT_NEAR(10.0, state.velocity, 1e-3);
        EXPECT_NEAR(10.0 * time, state.distance, 1e-3);

    };

    // set dynamic state
    state.xPosition = 100.0;
    state.yPosition = 200.0;
    state.yawAngle = M_PI / 8;
    state.velocity = 10.0;

    // run
    run(10.0);

    // check position
    EXPECT_NEAR(192.387953251, state.xPosition, 1e-9);
    EXPECT_NEAR(238.268343236, state.yPosition, 1e-9);

}



TEST_F(VehicleModelTest, MotionAccelerated) {


    // set checker
    checkStep = [this]() {

        // calculate s and v
        double s = 0.05 * time * time + 10.0 * time;
        double v = 10.0 + 0.1 * time;

        // check
        EXPECT_NEAR(0.1, state.acceleration, 1e-6);
        EXPECT_NEAR(v, state.velocity, 1e-6);
        EXPECT_NEAR(s, state.distance, 1e-3);

    };

    // set external force and init state
    resetFull();
    state.velocity = 10.0;
    parameters.externalRelForce = 0.1;

    // run
    run(10.0);


}


TEST_F(VehicleModelTest, Resistances) {

    // reset
    resetFull();

    // set resistance
    parameters.resistanceParameters[0] = 1.0;
    parameters.resistanceParameters[1] = 0.0;
    parameters.resistanceParameters[2] = 0.0;

    // set dynamic state
    state.velocity = 9.999999;

    // set checker
    checkStep = [this]() {

        // check until 10 s
        if(time >= 10.0 + EPS_SIM_TIME) {

            EXPECT_NEAR( 0.0, state.acceleration, 1e-9);
            EXPECT_NEAR( 0.0, state.velocity, 1e-6);
            EXPECT_NEAR(50.0, state.distance, 1e-3);

            return;

        }

        // calculate s and v
        double s = -0.5 * time * time + 10.0 * time;
        double v = 10.0 - time;

        // check
        EXPECT_NEAR(-1.0, state.acceleration, 1e-9);
        EXPECT_NEAR(v, state.velocity, 1e-6);
        EXPECT_NEAR(s, state.distance, 1e-3);

    };

    // run, one more step than 10 s
    run(11.0);

    // check
    EXPECT_NEAR(0.0, state.velocity, EPS_VELOCITY);
    EXPECT_NEAR(50.0, state.distance, 1e-3);

}


TEST_F(VehicleModelTest, ResistancesBackwards) {

    // reset
    resetFull();
    setShifter(ShifterPosition::REVERSE);

    // set resistance
    parameters.resistanceParameters[0] = 1.0;
    parameters.resistanceParameters[1] = 0.0;
    parameters.resistanceParameters[2] = 0.0;

    // set dynamic state
    state.velocity = -9.999999;

    // set checker
    checkStep = [this]() {

        // check until 10 s
        if(time >= 10.0 + EPS_SIM_TIME) {

            EXPECT_NEAR(  0.0, state.acceleration, 1e-9);
            EXPECT_NEAR(  0.0, state.velocity, 1e-6);
            EXPECT_NEAR(-50.0, state.distance, 1e-3);

            return;

        }

        // calculate s and v
        double s = -0.5 * time * time + 10.0 * time;
        double v = 10.0 - time;

        // check
        EXPECT_NEAR(1.0, state.acceleration, 1e-9);
        EXPECT_NEAR(-v, state.velocity, 1e-6);
        EXPECT_NEAR(-s, state.distance, 1e-3);

    };

    // run, one more step than 10 s
    run(11.0);

    // check
    EXPECT_EQ(0.0, state.velocity);
    EXPECT_NEAR(-50.0, state.distance, 1e-3);

}


TEST_F(VehicleModelTest, StartFromStandstill) {

    // set parameters
    parameters.maxRelReverseDrivePower =  11.9444444; // v_max = 10 m/s
    parameters.maxRelDrivePower        = 180.0;       // v_max = 60 m/s
    parameters.maxRelDriveTorque       =  10.0;       // friction limit
    parameters.maxRelBrakeTorque       =  10.0;       // friction limit

    // set resistance
    parameters.resistanceParameters[0] = 1.0;          // general resistance
    parameters.resistanceParameters[1] = 1.0 / 60.0;   // rolling resistance
    parameters.resistanceParameters[2] = 1.0 / 3600.0; // air drag

    // run with full drive power
    resetFull();
    setPedal(1.0);

    // run
    run(1000.0);

    // check
    EXPECT_NEAR( 0.0, state.acceleration, 1e-6);
    EXPECT_NEAR(60.0, state.velocity, 1e-3);


    // run with 20% drive power
    resetFull();
    setPedal(0.2);

    // run
    run(1000.0);

    // check
    EXPECT_NEAR( 0.0,   state.acceleration, 1e-6);
    EXPECT_NEAR(23.362, state.velocity, 1e-3);


    // run with 0% drive power
    resetFull();
    setPedal(0.0);

    // run
    run(1000.0);

    // check
    EXPECT_NEAR(0.0, state.acceleration, 1e-6);
    EXPECT_NEAR(0.0, state.velocity, 1e-3);


    // backwards with 10% drive power
    resetFull();
    setShifter(ShifterPosition::REVERSE);
    setPedal(0.5);

    // run
    run(1000.0);

    // check
    EXPECT_NEAR( 0.0,   state.acceleration, 1e-6);
    EXPECT_NEAR(-5.435, state.velocity, 1e-3);


    // backwards with 100% drive power
    resetFull();
    setShifter(ShifterPosition::REVERSE);
    setPedal(1.0);

    // run
    run(1000.0);

    // check
    EXPECT_NEAR(  0.0, state.acceleration, 1e-6);
    EXPECT_NEAR(-10.0, state.velocity, 1e-3);

}


TEST_F(VehicleModelTest, Steering) {

    // 100% of steering to the left
    resetFull();
    state.velocity = 5.0;
    setSteering(1.0);

    // run
    run(1.0);

    // check
    EXPECT_DOUBLE_EQ(0.1, state.curvature);
    EXPECT_DOUBLE_EQ(0.5, state.yawRate);



    // 10% of steering to the left
    resetFull();
    state.velocity = 10.0;
    setSteering(0.1);

    // run
    run(1.0);

    // check
    EXPECT_DOUBLE_EQ(0.01, state.curvature);
    EXPECT_DOUBLE_EQ(0.1, state.yawRate);



    // 10% of steering to the right
    resetFull();
    state.velocity = 10.0;
    setSteering(-0.1);

    // run
    run(1.0);

    // check
    EXPECT_DOUBLE_EQ(-0.01, state.curvature);
    EXPECT_DOUBLE_EQ(-0.1, state.yawRate);



    // 100% of steering to the right
    resetFull();
    state.velocity = 5.0;
    setSteering(-1.0);

    // run
    run(1.0);

    // check
    EXPECT_DOUBLE_EQ(-0.1, state.curvature);
    EXPECT_DOUBLE_EQ(-0.5, state.yawRate);


}


TEST_F(VehicleModelTest, DriveCurvesLeft) {

    bool left{};
    bool forwards{};

    // initialize
    init = [this, &left, &forwards]() {

        // set state
        state.xPosition = 0.0;
        state.yPosition = left ? -100.0 : 100.0;
        state.velocity = forwards ? 10.0 : -10.0;

        // set steering angle
        setSteering(left ? 0.1 : -0.1);

    };

    // set checker
    checkStep = [this, &left, &forwards]() {

        // calculate results
        auto v = forwards ? 10.0 : -10.0;
        auto p = (forwards ? 1.0 : -1.0) * (left ? 1.0 : -1.0) * time * 0.1;
        auto x = sin(p) * (left ?  100.0 : -100.0);
        auto y = cos(p) * (left ? -100.0 :  100.0);
        auto s = time * v;

        // check state
        EXPECT_NEAR(v, state.velocity, 1e-9);
        EXPECT_NEAR(p, state.yawAngle, 1e-6);
        EXPECT_NEAR(s, state.distance, 1e-6);
        EXPECT_NEAR(x, state.xPosition, 1e-3);
        EXPECT_NEAR(y, state.yPosition, 1e-3);

    };


    // reset, setup and run
    resetFull();
    left = true, forwards = true;
    run(1.0);


    // reset, setup and run
    resetFull();
    left = true, forwards = false;
    run(1.0);


    // reset, setup and run
    resetFull();
    left = false, forwards = true;
    run(1.0);


    // reset, setup and run
    resetFull();
    left = false, forwards = false;
    run(1.0);

}


TEST_F(VehicleModelTest, IndicatorStates) {

    indicateRight();
    EXPECT_EQ(IndicatorState::RIGHT, state.indicatorState);
    EXPECT_EQ(INFINITY, _indicatorTime);

    indicateLeft();
    EXPECT_EQ(IndicatorState::LEFT, state.indicatorState);
    EXPECT_EQ(INFINITY, _indicatorTime);

    hazard();
    EXPECT_EQ(IndicatorState::HAZARD, state.indicatorState);
    EXPECT_EQ(INFINITY, _indicatorTime);

    indicatorOff();
    EXPECT_EQ(IndicatorState::OFF, state.indicatorState);
    EXPECT_EQ(INFINITY, _indicatorTime);

    indicateRight(1.0);
    EXPECT_EQ(IndicatorState::RIGHT, state.indicatorState);
    EXPECT_EQ(1.0, _indicatorTime);

    indicateLeft(2.0);
    EXPECT_EQ(IndicatorState::LEFT, state.indicatorState);
    EXPECT_EQ(2.0, _indicatorTime);

    hazard();
    EXPECT_EQ(IndicatorState::HAZARD, state.indicatorState);
    EXPECT_EQ(INFINITY, _indicatorTime);

    indicateRight(3.0);
    EXPECT_EQ(IndicatorState::RIGHT, state.indicatorState);
    EXPECT_EQ(3.0, _indicatorTime);

    indicatorOff();
    EXPECT_EQ(IndicatorState::OFF, state.indicatorState);
    EXPECT_EQ(INFINITY, _indicatorTime);

    indicateRight(3.0);
    EXPECT_EQ(3.0, _indicatorTime);

    reset();
    EXPECT_EQ(INFINITY, _indicatorTime);

}


TEST_F(VehicleModelTest, IndicatorProzess) {

    // set indicators
    setStep = [this]() {

        if(isSimTime(5.0))
            indicateRight(3.0);
        else if(isSimTime(10.0))
            indicateLeft(2.0);
        else if(isSimTime(15.0))
            hazard();
        else if(isSimTime(19.0))
            indicatorOff();

    };

    // check indicators
    checkStep = [this]() {

        if(isInterval(0.0, 5.0))
            EXPECT_EQ(IndicatorState::OFF, state.indicatorState);
        else if(isInterval(5.0, 8.0))
            EXPECT_EQ(IndicatorState::RIGHT, state.indicatorState);
        else if(isInterval(8.0, 10.0))
            EXPECT_EQ(IndicatorState::OFF, state.indicatorState);
        else if(isInterval(10.0, 12.0))
            EXPECT_EQ(IndicatorState::LEFT, state.indicatorState);
        else if(isInterval(12.0, 15.0))
            EXPECT_EQ(IndicatorState::OFF, state.indicatorState);
        else if(isInterval(15.0, 19.0))
            EXPECT_EQ(IndicatorState::HAZARD, state.indicatorState);
        else if(isInterval(19.0, 21.0))
            EXPECT_EQ(IndicatorState::OFF, state.indicatorState);

    };

    // run simulation
    run(20.0);

}


TEST_F(VehicleModelTest, Shifter) {

    // switch to neutral
    setShifter(ShifterPosition::NEUTRAL);
    EXPECT_EQ(ShifterPosition::NEUTRAL, state.shifterPosition);

    // switch to drive
    setShifter(ShifterPosition::DRIVE);
    EXPECT_EQ(ShifterPosition::DRIVE, state.shifterPosition);

    // switch to reverse
    setShifter(ShifterPosition::REVERSE);
    EXPECT_EQ(ShifterPosition::REVERSE, state.shifterPosition);

    // switch to park
    setShifter(ShifterPosition::PARK);
    EXPECT_EQ(ShifterPosition::PARK, state.shifterPosition);

}


TEST_F(VehicleModelTest, WrongShiftingForwards) {

    // reset
    resetFull();
    setShifter(ShifterPosition::DRIVE);
    state.velocity = 10.0;

    // setting
    setStep = [this]() {

        if(isSimTime(1.0))
            setShifter(ShifterPosition::PARK);
        else if(isSimTime(2.0))
            setShifter(ShifterPosition::REVERSE);
        else if(isSimTime(3.0))
            setShifter(ShifterPosition::NEUTRAL);

    };

    // checking
    checkStep = [this]() {

        if(isInterval(0.0, 3.0))
            EXPECT_EQ(ShifterPosition::DRIVE, state.shifterPosition);
        else if(isInterval(3.0, 4.0))
            EXPECT_EQ(ShifterPosition::NEUTRAL, state.shifterPosition);

        // check velocity
        EXPECT_DOUBLE_EQ(10.0, state.velocity);

    };

    // run
    run(4.0);

}


TEST_F(VehicleModelTest, WrongShiftingBackwards) {

    // reset
    resetFull();
    setShifter(ShifterPosition::REVERSE);
    state.velocity = -5.0;

    // setting
    setStep = [this]() {

        if(isSimTime(1.0))
            setShifter(ShifterPosition::PARK);
        else if(isSimTime(2.0))
            setShifter(ShifterPosition::DRIVE);
        else if(isSimTime(3.0))
            setShifter(ShifterPosition::NEUTRAL);

    };

    // checking
    checkStep = [this]() {

        if(isInterval(0.0, 3.0))
            EXPECT_EQ(ShifterPosition::REVERSE, state.shifterPosition);
        else if(isInterval(3.0, 4.0))
            EXPECT_EQ(ShifterPosition::NEUTRAL, state.shifterPosition);

        // check velocity
        EXPECT_DOUBLE_EQ(-5.0, state.velocity);

    };

    // run
    run(4.0);

}


TEST_F(VehicleModelTest, WrongShiftingReverting) {

    // reset
    resetFull();
    setShifter(ShifterPosition::DRIVE);

    // set initial speed and brake pedal
    state.velocity = 5.0;
    setPedal(-0.5);

    // setting
    setStep = [this]() {

        if(isSimTime(2.0))
            setShifter(ShifterPosition::REVERSE);
        else if(isInterval(2.5, 5.0))
            setPedal(0.1);

    };

    // checking
    checkStep = [this]() {

        // check intervals
        if(isInterval(0.0, 2.0)) {
            EXPECT_LE(0.0, state.velocity);
            EXPECT_EQ(ShifterPosition::DRIVE, state.shifterPosition);
        } else if(isInterval(2.0, 2.5)) {
            EXPECT_NEAR(0.0, state.velocity, ZERO_VELOCITY);
            EXPECT_EQ(ShifterPosition::REVERSE, state.shifterPosition);
        }  else if(isInterval(2.5, 5.0)) {
            EXPECT_GE(0.0, state.velocity);
            EXPECT_EQ(ShifterPosition::REVERSE, state.shifterPosition);
        }

    };

    // run
    run(5.0);

}



//TEST_F(VehicleModelTest, ShiftingProcess) {
//
//    /**
//     * Process:
//     * -------------------------------------------
//     *     init: switch to P        -> P enabled
//     *  0s - 1s: pedal = -0.5       -> v = 0, a = 0
//     *       1s: switch to D        -> D enabled
//     *  1s - 2s: no pedal           -> v = 0, a = 0
//     *  2s - 4s: pedal = 0.1        -> v > 0, a > 0
//     *     2.5s: switch to R        -> no reaction, D enabled
//     *     2.6s: switch to P        -> no reaction, D enabled
//     *     2.7s: switch to N        -> N enabled, a < 0
//     *  4s - 5s: no pedal           -> a < 0
//     *  5s - 8s: pedal = -0.1       -> a < 0 -> a = 0, v = 0
//     *       8s: switch to N        -> N enabled, a = 0, v = 0
//     *     8.5s: switch to R        -> R enabled, a = 0, v = 0
//     *  8s - 9s: pedal = 0.0        -> a = 0, v = 0
//     * 9s - 12s: pedal = 0.1        -> a < 0, v < 0
//     *    10.5s: switch to D        -> no reaction, R
//     *    10.6s: switch to P        -> no reaction, R
//     *    10.7s: switch to N        -> N, deceleration
//     */
//
//
//
//
//    // reset
//    resetFull();
//
//    // set to park
//    setShifter(ShifterPosition::PARK);
//
//    // set step
//    setStep = [this]() {
//
//        // helpers
//        auto D = ShifterPosition::DRIVE;
//        auto R = ShifterPosition::REVERSE;
//        auto P = ShifterPosition::PARK;
//        auto N = ShifterPosition::NEUTRAL;
//
//        // reset both
//        setPedal(0.0);
//
//        // 0s..1s
//        if(isInterval(0.0, 1.0))
//            setPedal(-0.5);
//
//        // 1s
//        if(isSimTime(1.0))
//            setShifter(D);
//
//        // 1s..2s
//        if(isInterval(1.0, 2.0))
//            setPedal(0.0);
//
//        // 2s..4s
//        if(isInterval(1.0, 2.0))
//            setPedal(0.1);
//
//        // 2.5s
//        if(isSimTime(2.5))
//            setShifter(R);
//
//        // 2.6s
//        if(isSimTime(2.6))
//            setShifter(P);
//
//        // 2.7s
//        if(isSimTime(2.7))
//            setShifter(N);
//
//        // 4s..5s
//        if(isInterval(1.0, 2.0))
//            setPedal(0.0);
//
//    };
//
//    // check checker
//    checkStep = [this]() {
//
//    };
//
//    // run, one more step than 10 s
//    run(11.0);
//
//    // check
//    EXPECT_EQ(0.0, state.velocity);
//    EXPECT_NEAR(-50.0, state.distance, 1e-3);
//
//}

#pragma clang diagnostic pop