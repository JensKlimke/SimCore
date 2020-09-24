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

#include <simcore/ISynchronized.h>
#include <simcore/Loop.h>
#include <simcore/data/DataManager.h>
#include <simcore/timers/TimeIsUp.h>
#include <simcore/timers/RealTimeTimer.h>
#include <gtest/gtest.h>


class SimTest : public ::testing::Test, public sim::ISynchronized, protected sim::Loop {


public:

    std::function<void(double, double)> pre;

    double time = 0.0;
    double finalTime = 0.0;

    bool wasInitialized = false;
    int steps = 0;
    bool wasTerminated = false;


    SimTest() = default;
    ~SimTest() override = default;


    void setPrePostChecks() {

        // interrupt
        pre = [this] (double t, double dt) {

            // check time
            EXPECT_NEAR(1.0 + 0.01 * steps, t, EPS_SIM_TIME);

            // check time
            if(steps == 0)
                EXPECT_NEAR(0.0, dt, EPS_SIM_TIME);
            else
                EXPECT_NEAR(0.01, dt, EPS_SIM_TIME);

            // check state
            EXPECT_EQ(Loop::Status::RUNNING, this->getStatus());

        };

    }

    void checkFinalState(const sim::ITimer &timer, const sim::TimeIsUp &stop) {

        // check time and steps
        EXPECT_NEAR(11.0, timer.time(), EPS_SIM_TIME);
        EXPECT_NEAR(11.0, stop.getLastTime(), EPS_SIM_TIME);
        EXPECT_NEAR(11.0, time, EPS_SIM_TIME);
        EXPECT_NEAR(11.0, finalTime, EPS_SIM_TIME);
        EXPECT_EQ(1001, this->steps);

        // check status
        EXPECT_EQ(Loop::Status::STOPPED, this->getStatus());

        // check values
        EXPECT_TRUE(this->wasInitialized);
        EXPECT_TRUE(this->wasTerminated);

    }


    void setup(sim::ITimer &timer, sim::TimeIsUp &stop) {

        // set stop time
        stop.setStopTime(11.0);

        // set timer and stop condition
        this->setTimer(&timer);
        this->addStopCondition(&stop);

        // models
        this->addComponent(&stop);
        this->addComponent(this);

        // check status
        EXPECT_EQ(Loop::Status::STOPPED, this->getStatus());

    }


    void initialize(double initTime) override {

        time = initTime;
        steps = 0;

        wasInitialized = true;
        wasTerminated = false;

    }


    void step(double t, double dt) override {

        // call pre step
        if(pre)
            pre(t, dt);

        // main step
        time += dt;
        steps++;

    }


    void terminate(double simTime) override {

        wasTerminated = true;
        finalTime = simTime;

    }

};


TEST_F(SimTest, SimulationLifeCycle) {

    using namespace ::sim;

    // set check during running
    setPrePostChecks();

    // create objects
    BasicTimer timer;
    TimeIsUp stop;

    // set timer parameters
    timer.setTimeStepSize(0.01);
    timer.setStartTime(1.0);

    // setup
    setup(timer, stop);

    // run simulation
    this->run();

    // check final state
    checkFinalState(timer, stop);

}


TEST_F(SimTest, MultipleRuns) {

    using namespace ::sim;

    // set check during running
    setPrePostChecks();

    // create objects
    BasicTimer timer;
    TimeIsUp stop;

    // set timer parameters
    timer.setTimeStepSize(0.01);
    timer.setStartTime(1.0);

    // setup
    setup(timer, stop);

    // run ten times
    for(unsigned int i = 0; i < 10; ++i) {

        // run simulation
        this->run();

        // check final state
        checkFinalState(timer, stop);

    }

}


TEST_F(SimTest, RealTimeSimulation) {

    using namespace ::sim;

    // set check during running
    setPrePostChecks();

    // create objects
    RealTimeTimer timer;
    TimeIsUp stop;

    // set timer parameters
    timer.setAcceleration(10.0);
    timer.setTimeStepSize(0.01);
    timer.setStartTime(1.0);

    // setup
    setup(timer, stop);

    // run simulation
    this->run();

    // check final state
    checkFinalState(timer, stop);

}


TEST_F(SimTest, NotSetProperly) {

    using namespace ::sim;

    // erroneous calls before initialization
    EXPECT_THROW(this->run(), ProcessException);
    EXPECT_THROW(this->stop(), ProcessException);
    EXPECT_THROW(this->_execute(), ProcessException);

    // create objects
    BasicTimer timer;
    timer.setTimeStepSize(0.01);

    // interrupt
    pre = [this] (double t, double dt) {

        // at t=0.1 s
        if(t > 0.099999) {

            // try to initialize during run
            EXPECT_THROW(Loop::_initialize(), ProcessException);

            // stop
            stop();

        }

    };

    // set timer and stop condition
    this->setTimer(&timer);
    this->addComponent(this);

    // run simulation
    this->run();

}


#pragma clang diagnostic pop