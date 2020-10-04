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
// Created by Jens Klimke on 2020-09-10.
//

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <cmath>
#include <simcore/Loop.h>
#include <simcore/Model.h>
#include <simcore/timers/RealTimeTimer.h>
#include <simcore/timers/TimeIsUp.h>
#include <gtest/gtest.h>


class TimerTest : public ::testing::Test, public sim::Model {

public:

    // create objects
    sim::ITimer *timer = nullptr;
    sim::TimeIsUp stop{};
    sim::Loop loop{};
    std::function<void(double, double)> check{};

    double initTime{};
    double termTime{};
    double lastTime{};
    unsigned int steps = 0;

    void SetUp() override {

        // step size
        this->setTimeStepSize(0.1);

        // set parameters
        stop.setStopTime(10.0);

        // set timer and stop condition
        loop.addStopCondition(&stop);

        // models
        loop.addComponent(&stop);
        loop.addComponent(this);

    }

    void TearDown() override {

        delete timer;

    }


protected:

    void initialize(double t) override {

        // save time
        initTime = t;

    }

    void step(double t, double dt) override {

        // save last time
        lastTime = t;

        // check time
        EXPECT_NEAR(1.0 + steps * 0.1, t, 0.1);
        EXPECT_NEAR(steps > 0 ? 0.1 : 0.0, dt, 0.1);

        // increment
        steps++;

    }

    void terminate(double t) override {

        // save time
        termTime = t;

    }

};


TEST_F(TimerTest, BasicTimer) {

    // create timer
    auto bt = new sim::BasicTimer;
    bt->setTimeStepSize(0.01);
    bt->setStartTime(1.0);

    // set timer
    loop.setTimer(bt);

    // run
    loop.run();

    // check
    EXPECT_NEAR(1.0, initTime, 1e-3);
    EXPECT_NEAR(10.0, termTime, 1e-3);
    EXPECT_NEAR(10.0, lastTime, 1e-3);

}


TEST_F(TimerTest, RealTime) {

    // create timer
    auto bt = new sim::RealTimeTimer;
    bt->setAcceleration(100.0);
    bt->setTimeStepSize(0.01);
    bt->setStartTime(1.0);

    // set timer
    loop.setTimer(bt);

    // start real time
    auto start = std::chrono::system_clock::now();

    // run
    loop.run();

    // stop time
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start);
    auto runTime = static_cast<double>(elapsed.count()) / 1000.0;

    // check
    EXPECT_NEAR(0.09, runTime, 0.01);
    EXPECT_NEAR(1.0, initTime, 0.1);
    EXPECT_NEAR(10.0, termTime, 0.1);
    EXPECT_NEAR(10.0, lastTime, 0.1);

}

#pragma clang diagnostic pop