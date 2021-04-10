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
#include <simcore/BasicSimulation.h>



class BasicSimulationTest : public ::testing::Test, public simcore::BasicSimulation {

public:

    BasicSimulationTest() = default;
    ~BasicSimulationTest() override = default;

};


TEST_F(BasicSimulationTest, Status) {

    // no timer set
    EXPECT_THROW(Loop::run(), ProcessException);

    // create generic component
    auto gen0 = createGenericComponent();
    auto gen1 = createGenericComponent();

    // check components
    EXPECT_EQ(2, genericComponent.size());
    EXPECT_EQ(genericComponent.front().get(), gen0);
    EXPECT_EQ(genericComponent.back().get(), gen1);

    // counters
    unsigned int i = 100, j = 200, k = 300;

    // init function
    gen0->initFnc = [this, &i] (double t) {

        // counter
        i++;

        // check status
        EXPECT_EQ(Status::INITIALIZING, getStatus());

    };

    // step function
    gen0->stepFnc = [this, &j] (double t, double) {

        // check if first time step, then count
        if(simcore::IComponent::timeEq(t, 0.0))
            j++;

        // check status
        EXPECT_EQ(Status::RUNNING, getStatus());

    };

    // termination function
    gen0->termFnc = [this, &k] (double t) {

        // counter
        k++;

        // check status
        EXPECT_EQ(Status::TERMINATING, getStatus());

    };


    // counter for 2nd component
    gen1->initFnc = [&i] (double t) { i += 10; };
    gen1->termFnc = [&k] (double t) { k += 10; };
    gen1->stepFnc = [this, &j] (double t, double) {

        // check if first time step, then count
        if(simcore::IComponent::timeEq(t, 1.0))
            j += 10;

        // check status
        EXPECT_EQ(Status::RUNNING, getStatus());

    };


    // init timers
    create(10.0, 0.01);

    // check status
    EXPECT_EQ(Status::STOPPED, getStatus());
    EXPECT_EQ(100, i);
    EXPECT_EQ(200, j);
    EXPECT_EQ(300, k);

    // run
    Loop::run();

    // check status
    EXPECT_EQ(Status::STOPPED, getStatus());
    EXPECT_EQ(111, i);
    EXPECT_EQ(211, j);
    EXPECT_EQ(311, k);

    // run
    Loop::run();

    // check status
    EXPECT_EQ(Status::STOPPED, getStatus());
    EXPECT_EQ(122, i);
    EXPECT_EQ(222, j);
    EXPECT_EQ(322, k);

}
