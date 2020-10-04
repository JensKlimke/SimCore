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
// Created by Jens Klimke on 2019-03-19.
//

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <gtest/gtest.h>
#include <simcore/ISynchronized.h>
#include <simcore/Loop.h>
#include <simcore/timers/TimeIsUp.h>
#include <simcore/timers/RealTimeTimer.h>
#include <simcore/testing/SimulationTest.h>


class DummyModel : public sim::Model {

protected:

    double startTime{};
    double endTime{};
    double timeStepSize{};
    double timeAccuracy{};

    double time{};
    unsigned int steps{};


    void initialize(double t) override {

        // check initial time
        EXPECT_NEAR(startTime, t, timeAccuracy);

        // set time
        steps = 0;
        time = t;

    }

    void step(double t, double dt) override {

        // check
        EXPECT_NEAR(t, startTime + steps * timeStepSize, timeAccuracy);
        EXPECT_NEAR(steps > 0 ? dt : 0.0, t - time, timeAccuracy);

        // save
        steps++;
        time = t;

    }

    void terminate(double t) override {

        // termination time
        EXPECT_NEAR(endTime, t, timeAccuracy);

    }

};


class LifeCycleTest : public ::testing::Test, public sim::testing::SimulationTest<DummyModel> {


public:

    LifeCycleTest() = default;
    ~LifeCycleTest() override = default;

    void createSimulation(double endTime, double timeStepSize, double startTime, double timeAccuracy, bool runAndCheck) {

        // save attributes
        DummyModel::endTime = endTime;
        DummyModel::startTime = startTime;
        DummyModel::timeStepSize = timeStepSize;
        DummyModel::timeAccuracy = timeAccuracy;

        // create
        SimulationTest::create(endTime, timeStepSize, startTime);

        // set time step size
        setTimeStepSize(timeStepSize);

        // run
        if(runAndCheck) {

            // run
            run();

            // calculate number or steps
            auto totSteps = (unsigned int) floor((endTime - startTime) / timeStepSize) + 1;

            // check
            EXPECT_EQ(totSteps, DummyModel::steps);                 // steps performed
            EXPECT_NEAR(endTime, DummyModel::time, timeAccuracy);   // last time


        }

    }

};


TEST_F(LifeCycleTest, Simple) {

    // create and run
    createSimulation(10.0, 0.1, 0.0, EPS_SIM_TIME, false);

    // run
    run();

    // check
    EXPECT_EQ(101, DummyModel::steps);                  // steps performed
    EXPECT_NEAR(10.0, DummyModel::time, EPS_SIM_TIME);  // last time

}


TEST_F(LifeCycleTest, StartTime) {

    // create, run and check
    createSimulation(10.0, 0.01, 1.0, EPS_SIM_TIME, true);

}


TEST_F(LifeCycleTest, SmallTimeStepSize) {

    // create, run and check
    createSimulation(1e-5, 1e-8, 0.0, EPS_SIM_TIME, true);

}


TEST_F(LifeCycleTest, MultipleCreations) {

    // create, run and check
    createSimulation(10.0, 0.1,  0.0, EPS_SIM_TIME, true);
    createSimulation(10.0, 0.1,  1.0, EPS_SIM_TIME, true);
    createSimulation(10.0, 0.01, 0.0, EPS_SIM_TIME, true);
    createSimulation(100.0, 0.1, 2.0, EPS_SIM_TIME, true);

}


TEST_F(LifeCycleTest, MultipleRuns) {

    // create, run and check
    createSimulation(10.0, 0.1,  1.0, EPS_SIM_TIME, false);

    // run first time
    run();

    // check
    EXPECT_EQ(91, DummyModel::steps);                  // steps performed
    EXPECT_NEAR(10.0, DummyModel::time, EPS_SIM_TIME);  // last time


    // run second time
    run();

    // check
    EXPECT_EQ(91, DummyModel::steps);                  // steps performed
    EXPECT_NEAR(10.0, DummyModel::time, EPS_SIM_TIME);  // last time


    // change times
    _timer->setStartTime(2.0);
    _timer->setTimeStepSize(0.01); // this is only for loop ...
    _stopTimer->setStopTime(20.0);

    // reset attributes
    DummyModel::timeStepSize = 0.1; // ... for model dt=0.1 is kept
    DummyModel::startTime = 2.0;
    DummyModel::endTime = 20.0;

    // run third time
    run();

    // check
    EXPECT_EQ(181, DummyModel::steps);                  // steps performed
    EXPECT_NEAR(20.0, DummyModel::time, EPS_SIM_TIME);  // last time

}


TEST_F(LifeCycleTest, RealTime) {

    // set attributes manually
    DummyModel::startTime = 1.0;
    DummyModel::endTime = 10.0;
    DummyModel::timeStepSize = 0.1;
    DummyModel::timeAccuracy = 0.1;

    // create simulation
    setTimeStepSize(0.1);
    create(10.0, 0.1, 1.0, 100.0);

    // start timer
    auto start = std::chrono::system_clock::now();

    // run
    run();

    // stop time
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - start);
    auto runTime = static_cast<double>(elapsed.count()) / 1e6;

    // check
    EXPECT_NEAR(0.09, runTime, 1e-2);

}


#pragma clang diagnostic pop