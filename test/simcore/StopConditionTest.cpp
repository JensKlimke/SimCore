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
// Created by Jens Klimke on 2020-08-10.
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


class StopConditionTest : public ::testing::Test, public ::sim::ISynchronized, public ::sim::IStopCondition {


protected:

    sim::Loop loop{};
    std::function<void(double, double)> callback{};


public:

    StopConditionTest() = default;
    ~StopConditionTest() override = default;


    void initialize(double initTime) override {}


    void step(double t, double dt) override {

        // call
        if(t > 9.999 & t < 10.001)
            this->callback(t, dt);

    }


    void terminate(double simTime) override {}

};


TEST_F(StopConditionTest, Interrupt) {

    // interrupt
    callback = [this] (double t, double dt) {
        loop.stop();
    };

    using namespace ::sim;

    // create loop
    loop.addComponent(this);
    loop.addStopCondition(this);

    // set timer
    BasicTimer timer;
    timer.setTimeStepSize(0.01);
    loop.setTimer(&timer);

    // run loop
    loop.run();

    // check
    EXPECT_EQ(IStopCondition::StopCode::NONE, this->getCode());
    EXPECT_NEAR(10.0, timer.time(), EPS_SIM_TIME);

}


TEST_F(StopConditionTest, Success) {

    // interrupt
    callback = [this] (double t, double dt) {
        this->success();
    };

    using namespace ::sim;

    // create loop
    loop.addComponent(this);
    loop.addStopCondition(this);

    // set timer
    BasicTimer timer;
    timer.setTimeStepSize(0.01);
    loop.setTimer(&timer);

    // run loop
    loop.run();

    // check
    EXPECT_EQ(IStopCondition::StopCode::OBJECTIVES_REACHED, this->getCode());
    EXPECT_NEAR(10.0, timer.time(), EPS_SIM_TIME);

}


TEST_F(StopConditionTest, Fail) {

    // interrupt
    callback = [this] (double t, double dt) {
        this->failed();
    };

    using namespace ::sim;

    // create loop
    loop.addComponent(this);
    loop.addStopCondition(this);

    // set timer
    BasicTimer timer;
    timer.setTimeStepSize(0.01);
    loop.setTimer(&timer);

    // run loop
    loop.run();

    // check
    EXPECT_EQ(IStopCondition::StopCode::OBJECTIVES_MISSED, this->getCode());
    EXPECT_NEAR(10.0, timer.time(), EPS_SIM_TIME);

}



TEST_F(StopConditionTest, End) {

    // interrupt
    callback = [this] (double t, double dt) {
        this->end();
    };

    using namespace ::sim;

    // create loop
    loop.addComponent(this);
    loop.addStopCondition(this);

    // set timer
    BasicTimer timer;
    timer.setTimeStepSize(0.01);
    loop.setTimer(&timer);

    // run loop
    loop.run();

    // check
    EXPECT_EQ(IStopCondition::StopCode::SIM_ENDED, this->getCode());
    EXPECT_NEAR(10.0, timer.time(), EPS_SIM_TIME);

}


#pragma clang diagnostic pop