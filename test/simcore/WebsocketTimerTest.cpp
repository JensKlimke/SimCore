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
// Created by Jens Klimke on 2020-08-21.
//

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <simcore/ISynchronized.h>
#include <simcore/Loop.h>
#include <simcore/timers/TimeIsUp.h>
#include <simcore/timers/WebSocketTimer.h>
#include <simcore/data/TimeReporter.h>
#include <gtest/gtest.h>


class WebsocketTimerTest : public ::testing::Test, public sim::ISynchronized, protected sim::Loop {


public:

    double time = 0.0;
    double finalTime = 0.0;


    WebsocketTimerTest() = default;
    ~WebsocketTimerTest() override = default;


    void initialize(double initTime) override {

        // set init time
        time = initTime;

    }


    void step(double t, double dt) override {

        // main step
        time += dt;

    }


    void terminate(double simTime) override {

        // set final time
        finalTime = simTime;

    }

};


TEST_F(WebsocketTimerTest, SyncTest) {

    using namespace ::sim;

    // create timer
    WebSocketTimer timer;
    timer.setAcceleration(2.0);
    timer.setTimeStepSize(0.1);

    // end of loop
    TimeIsUp stop;
    stop.setStopTime(10.0);

    // set timer and add this as component
    this->setTimer(&timer);
    this->addComponent(this);
    this->addStopCondition(&stop);
    this->addComponent(&stop);

    // run loop
    this->run();

    // check
    EXPECT_NEAR(10.0, finalTime, 1e-3);

}



#pragma clang diagnostic pop