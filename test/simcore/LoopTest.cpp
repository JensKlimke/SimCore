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
#include <simcore/Loop.h>
#include <simcore/Model.h>
#include <simcore/timers/TimeIsUp.h>
#include <simcore/timers/RealTimeTimer.h>


class LoopTest : public ::testing::Test, public sim::Loop, public sim::Model, public sim::IStopCondition {

public:

    bool isInitialized = false;
    bool isExecuted = false;
    bool isTerminated = false;

    LoopTest() = default;
    ~LoopTest() override = default;

    void initialize(double t) override {

        // set
        isInitialized = true;

        // check
        EXPECT_EQ(Status::STOPPED, this->getStatus());

    }

    void step(double t, double dt) override {

        // set
        isExecuted = true;

        // check state
        EXPECT_EQ(Status::RUNNING, this->getStatus());

        // at t=0.1 s
        if(t > 0.099999) {

            // try to init
            EXPECT_THROW(_initialize(), sim::ProcessException);

            // stop simulation
            abort();

        }

    }

    void terminate(double t) override {

        // set
        isTerminated = true;

        // check
        EXPECT_EQ(Status::STOPPED, this->getStatus());

    }

};


TEST_F(LoopTest, NotSetProperly) {

    using namespace ::sim;

    // erroneous calls before initialization
    EXPECT_THROW(run(), ProcessException);
    EXPECT_THROW(abort(), ProcessException);
    EXPECT_THROW(_execute(), ProcessException);

    // check status
    EXPECT_EQ(Status::STOPPED, getStatus());

    // create objects
    BasicTimer timer;
    timer.setTimeStepSize(0.01);

    // set timer and stop condition
    setTimer(&timer);

    // add component, not allowed to add component twice
    addComponent(this);
    EXPECT_THROW(addComponent(this), sim::SetupException);

    // add stop condition, not allowed to add stop condition twice
    addStopCondition(this);
    EXPECT_THROW(addStopCondition(this), sim::SetupException);

    // run simulation
    EXPECT_THROW(run(), sim::SetupException);

    // set time step size and run again
    setTimeStepSize(0.01);
    run();

    // check status
    EXPECT_EQ(Status::STOPPED, getStatus());
    EXPECT_TRUE(isInitialized);
    EXPECT_TRUE(isExecuted);
    EXPECT_TRUE(isTerminated);

}


#pragma clang diagnostic pop