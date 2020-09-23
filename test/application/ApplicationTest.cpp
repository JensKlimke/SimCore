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

#include <gtest/gtest.h>
#include <simcore/timers/RealTimeTimer.h>
#include <simcore/framework/Simulation.h>
#include "SampleApplication.h"

class ApplicationTest : public ::testing::Test, protected SampleApplication, protected Simulation {

protected:

    sim::ITimer *_timer;


public:

    void SetUp() override {

    }


    void TearDown() override {

        delete _timer;

    }

};


TEST_F(ApplicationTest, Create) {

    // reserve space for sim time container
    this->simTimes.reserve(100);

    // set timer
    this->createTimer(0.01);

    // set simulation time
    this->setSimulationTime(10.0);

    // register this application
    this->setTimeStepSize(0.1);
    this->component(this);

    // execute
    this->execute();

    // init and termination time
    EXPECT_NEAR(0.0, this->initTime, EPS_SIM_TIME);
    EXPECT_NEAR(10.0, this->termTime, EPS_SIM_TIME);

    // TODO: check times

}

