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
#include <simcore/testing/Simulation.h>


class LifeCycleTest : public ::testing::Test, public sim::testing::Simulation, public sim::ISynchronized {


public:

    double timeAccuracy{};
    double startTime{};
    double timeStepSize{};
    double endTime{};
    double time{};

    LifeCycleTest() = default;

    ~LifeCycleTest() override = default;


    void createSimulation(double setEndTime, double setTSS, double setStartTime, double setTimeAccuracy) {

        // set parameters
        this->timeAccuracy = setTimeAccuracy;
        this->startTime = setStartTime;
        this->endTime = setEndTime;
        this->timeStepSize = setTSS;

        // create simulation
        create(setEndTime, setTSS, setStartTime);

        // add this
        setTimeStepSize(setTSS);
        addComponent(this);

    }


    void initialize(double t) override {

        // run init of simulation
        preInitialize(t);

        // save time
        time = t;

        // check initial time
        EXPECT_NEAR(startTime, t, timeAccuracy);

    }

    void step(double t, double dt) override {

        // run pre-step of simulation
        preStep(t, dt);

        // check
        EXPECT_NEAR(t, startTime + getTimeStep().steps * timeStepSize, timeAccuracy);
        EXPECT_NEAR(dt, t - time, timeAccuracy);
        EXPECT_NEAR(dt, getTimeStep().deltaTime, timeAccuracy);

        // run pre-step of simulation
        postStep(t, dt);

        // save time
        time = t;

    }

    void terminate(double t) override {

        // run pre-terminate
        preTerminate(t);

        // termination time
        EXPECT_NEAR(endTime, t, timeAccuracy);

        // calculate number or steps
        auto totSteps = (unsigned int) floor((endTime - startTime) / timeStepSize) + 1;

        // check
        EXPECT_EQ(totSteps, getTimeStep().steps);                   // steps performed
        EXPECT_NEAR(endTime, getTimeStep().time, timeAccuracy);     // last time
        EXPECT_NEAR(endTime, getTimeStep().termTime, timeAccuracy); // termination

    }

};


TEST_F(LifeCycleTest, Simple) {

    // create and run
    createSimulation(10.0, 0.1, 0.0, EPS_SIM_TIME);

    // run
    run();

}



TEST_F(LifeCycleTest, StartTime) {

    // create, run and check
    createSimulation(10.0, 0.01, 1.0, EPS_SIM_TIME);

}


TEST_F(LifeCycleTest, SmallTimeStepSize) {

    // create, run and check
    createSimulation(1e-5, 1e-8, 0.0, EPS_SIM_TIME);

}


TEST_F(LifeCycleTest, MultipleCreations) {

    // create, run and check
    createSimulation(10.0, 0.1, 0.0, EPS_SIM_TIME);
    createSimulation(10.0, 0.1, 1.0, EPS_SIM_TIME);
    createSimulation(10.0, 0.01, 0.0, EPS_SIM_TIME);
    createSimulation(100.0, 0.1, 2.0, EPS_SIM_TIME);

}


TEST_F(LifeCycleTest, MultipleRuns) {

    // create, run and check
    createSimulation(10.0, 0.1, 1.0, EPS_SIM_TIME);

    // run first time
    run();

    // check
    EXPECT_EQ(91, getTimeStep().steps);                  // steps performed
    EXPECT_NEAR(10.0, getTimeStep().time, EPS_SIM_TIME); // last time


    // run second time
    run();

    // check
    EXPECT_EQ(91, getTimeStep().steps);                  // steps performed
    EXPECT_NEAR(10.0, getTimeStep().time, EPS_SIM_TIME); // last time


    // change times
    createSimulation(20.0, 0.01, 2.0, EPS_SIM_TIME);
    setTimeStepSize(0.1);

    // reset attributes
    timeStepSize = 0.1;
    startTime = 2.0;
    endTime = 20.0;

    // run third time
    run();

    // check
    EXPECT_EQ(181, getTimeStep().steps);                  // steps performed
    EXPECT_NEAR(20.0, getTimeStep().time, EPS_SIM_TIME);  // last time

}


TEST_F(LifeCycleTest, RealTime) {

    // set attributes manually
    startTime = 1.0;
    endTime = 10.0;
    timeStepSize = 0.1;
    timeAccuracy = 0.1;

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