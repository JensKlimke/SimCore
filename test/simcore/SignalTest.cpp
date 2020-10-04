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
// Created by Jens Klimke on 2019-04-16.
//

#include <simcore/IComponent.h>
#include <simcore/Loop.h>
#include <simcore/value/SignalCurve.h>
#include <simcore/value/ValueExceed.h>
#include <simcore/value/StopIf.h>
#include <simcore/value/ValueOutOfTube.h>
#include <simcore/timers/BasicTimer.h>
#include <simcore/timers/TimeIsUp.h>
#include <gtest/gtest.h>


class SignalTest : public ::testing::Test, public sim::IComponent {

protected:

    // create objects
    ::sim::BasicTimer timer;
    ::sim::TimeIsUp stop;
    ::sim::Loop sim;

    double x     = 0.0;
    double value = 0.0;
    bool flag    = false;


    void SetUp() override {

        // set parameters
        timer.setTimeStepSize(1.0);
        stop.setStopTime(10.0);

        // set timer and stop condition
        sim.setTimer(&timer);
        sim.addStopCondition(&stop);

        // models
        sim.addComponent(&stop);

    }


private:

    void _exec(double simTime) override {

        x += 1.0;
        value = x * x;
        flag = x >= 10.0;

    }


public:

    void _init(double initTime) override {
        value = 0.0;
        x = 0.0;
        flag = false;
    }

    void _term(double simTime) override {
    }

};


TEST_F(SignalTest, ValueExceed) {

    // check value to exceed
    sim::value::ValueExceed<double> ex;
    ex.setPointerAndLimit(&value, 25.1, sim::value::ValueExceed<double>::Mode::OBJECTIVES_MISSED);

    // add to sim
    sim.addComponent(&ex);
    sim.addStopCondition(&ex);

    // add value limiter
    sim.addComponent(this);

    // run sim
    sim.run();
    EXPECT_EQ(ex.getCode(), ::sim::IStopCondition::StopCode::OBJECTIVES_MISSED);
    EXPECT_DOUBLE_EQ(6.0, timer.time());
    EXPECT_DOUBLE_EQ(49.0, value);


    // change mode
    ex.setPointerAndLimit(&value, 25.1);

    // run sim again
    sim.run();
    EXPECT_EQ(ex.getCode(), ::sim::IStopCondition::StopCode::SIM_ENDED);
    EXPECT_EQ(6.0, timer.time());
    EXPECT_DOUBLE_EQ(49.0, value);


    // change limit
    ex.setPointerAndLimit(&value, 100.1);

    // run sim again
    sim.run();
    EXPECT_EQ(ex.getCode(), ::sim::IStopCondition::StopCode::NONE);
    EXPECT_EQ(10.0, timer.time());
    EXPECT_DOUBLE_EQ(121.0, value);

}


TEST_F(SignalTest, StopIf) {

    // check value to exceed
    sim::value::StopIf ex;
    ex.setPointer(&flag);

    // add to sim
    sim.addComponent(&ex);
    sim.addStopCondition(&ex);

    // add value limiter
    sim.addComponent(this);

    // run sim
    sim.run();
    EXPECT_EQ(ex.getCode(), ::sim::IStopCondition::StopCode::SIM_ENDED);
    EXPECT_DOUBLE_EQ(10.0, timer.time());
    EXPECT_TRUE(flag);


    // change mode
    ex.setStopMode(::sim::IStopCondition::StopCode::OBJECTIVES_REACHED);

    // run sim again
    sim.run();
    EXPECT_EQ(ex.getCode(), ::sim::IStopCondition::StopCode::OBJECTIVES_REACHED);
    EXPECT_EQ(10.0, timer.time());
    EXPECT_TRUE(flag);

}


TEST_F(SignalTest, OutOfTube) {

    // check value to exceed
    sim::value::ValueOutOfTube tube;
    tube.defineLower({0.0, 10.0}, {-0.1, -0.1});
    tube.defineUpper({0.0, 10.0}, { 1.1,  1.1});

    // set values
    tube.setValues(&x, &value);

    // add to sim
    sim.addComponent(&tube);
    sim.addStopCondition(&tube);

    // add value limiter
    sim.addComponent(this);


    // run sim
    sim.run();
    EXPECT_EQ(tube.getCode(), ::sim::IStopCondition::StopCode::OBJECTIVES_MISSED);
    EXPECT_EQ(2.0, timer.time());
    EXPECT_DOUBLE_EQ(9.0, value);


    // change limits
    tube.defineLower({0.0, 2.5, 5.0, 7.5, 10.0}, {-0.1, 0.0,   6.25, 25.0,   56.25});
    tube.defineUpper({0.0, 2.5, 5.0, 7.5, 10.0}, { 0.1, 6.35, 25.1,  56.35, 100.1 });

    // run sim again
    sim.run();
    EXPECT_EQ(tube.getCode(), ::sim::IStopCondition::StopCode::NONE);
    EXPECT_EQ(10.0, timer.time());
    EXPECT_DOUBLE_EQ(121.0, value);

}
