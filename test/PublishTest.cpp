//
// Copyright (c) 2019 Jens Klimke <jens.klimke@rwth-aachen.de>. All rights reserved.
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
// Created by Jens Klimke on 2019-04-22.
//

#include <simcore/Loop.h>
#include <simcore/Model.h>
#include <simcore/timers/RealTimeTimer.h>
#include <simcore/timers/TimeIsUp.h>
#include <simcore/socket/DataPublisher.h>
#include <gtest/gtest.h>

class PublishTest : public ::testing::Test, public sim::IComponent {

public:

    double time  = 0.0;


protected:

    // create objects
    RealTimeTimer timer;
    TimeIsUp stop;
    ::sim::Loop loop;


    void SetUp() override {

        // set parameters
        timer.setTimeStepSize(0.1);
        stop.setStopTime(10.0);

        // set timer and stop condition
        loop.setTimer(&timer);
        loop.addStopCondition(&stop);

        // models
        loop.addComponent(&stop);

    }


private:

    bool step(double simTime) override {

        // write data into variable
        time = simTime;
        return true;

    }


public:

    void initialize(double initTime) override {

        time = initTime;

    }

    void terminate(double simTime) override {

        time = simTime;

    }


};



TEST_F(PublishTest, Publisher) {

    // create out-stream
    std::stringstream ostr;

    // register this (this updates the time variable)
    loop.addComponent(this);

    // TODO: test

    // initialize simulation
    EXPECT_NO_THROW(loop.run());

}
