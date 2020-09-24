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
#include "TrafficSimulation.h"
#include "SampleApplication.h"


class ApplicationTest : public ::testing::Test, protected TrafficSimulation {

public:

    void SetUp() override {

    }


    void TearDown() override {}

};



TEST_F(ApplicationTest, Create) {

    // create simulation
    create(10.0, 0.01);

    // create sample application
    SampleApplication model{};
    model.setTimeStepSize(0.1);
    addExternalComponent(&model);

    // run the simulation
    run();

    // init and termination time
    EXPECT_NEAR(0.0, model.initTime, EPS_SIM_TIME);
    EXPECT_NEAR(10.0, model.termTime, EPS_SIM_TIME);

    // check no of saved time steps
    EXPECT_EQ(101, model.timeSteps.size());

    // check time steps
    unsigned long i = 0;
    for(auto &step : model.timeSteps) {

        // check time and time step size
        EXPECT_NEAR(0.1 * i, step.first, EPS_SIM_TIME);
        EXPECT_NEAR(i == 0 ? 0.0 : 0.1, step.second, EPS_SIM_TIME);

        // increment counter
        ++i;

    }

}

