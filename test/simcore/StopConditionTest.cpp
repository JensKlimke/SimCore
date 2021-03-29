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


#include <functional>
#include <gtest/gtest.h>
#include <simcore/IComponent.h>
#include <simcore/Loop.h>
#include <simcore/timers/BasicTimer.h>
#include <simcore/timers/TimeIsUp.h>


class StopConditionTest : public ::testing::Test, public simcore::StopCondition {


public:

    // create timer
    BasicTimer timer;
    TimeIsUp stop;

    // step function
    std::function<void (double)> stepFnc;

    LoopTest() = default;
    ~LoopTest() override = default;

    void SetUp() override {

        // add this
        addComponent(this);

    }

    void initialize(double initTime) override {

        if(initFnc)
            initFnc(initTime);

    }

    void terminate(double simTime) override {

        if(termFnc)
            termFnc(simTime);

    }

    void step(double simTime) override {

        // step function
        if(stepFnc)
            stepFnc(simTime);

        // stop after 10 steps
        if(_steps == 10)
            Loop::stop();

    }


    void initTimers(double stopTime) {

        // stop condition
        stop.setStopTime(stopTime);
        addStopCondition(&stop);

        // set timer
        timer.setTimeStepSize(0.1);
        setTimer(&timer);

        // add components
        addComponent(&stop);

    }

};


TEST_F(LoopTest, UnproperUsage) {

    // no timer set
    EXPECT_THROW(run(), ProcessException);

    // step function
    stepFnc = [this] (double t) {
        EXPECT_EQ(Status::RUNNING, getStatus());
    };

    // init function
    initFnc = [this] (double t) {
        EXPECT_EQ(Status::INITIALIZING, getStatus());
    };

    // termination function
    termFnc = [this] (double t) {
        EXPECT_EQ(Status::TERMINATING, getStatus());
    };

    // init timers
    initTimers(10.0);

    // check status
    EXPECT_EQ(Status::STOPPED, getStatus());

    // run
    run();

    // check status
    EXPECT_EQ(Status::STOPPED, getStatus());

}