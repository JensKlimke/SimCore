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
#include <simcore/BasicSimulation.h>



class LoopTest : public ::testing::Test, public sim::BasicSimulation {

public:

    LoopTest() = default;
    ~LoopTest() override = default;

};


TEST_F(LoopTest, Status) {

    // no timer set
    EXPECT_THROW(Loop::run(), ProcessException);

    // create generic component
    auto gen = createGenericComponent();

    // step function
    gen->stepFnc = [this] (double t) {
        EXPECT_EQ(Status::RUNNING, getStatus());
    };

    // init function
    gen->initFnc = [this] (double t) {
        EXPECT_EQ(Status::INITIALIZING, getStatus());
    };

    // termination function
    gen->termFnc = [this] (double t) {
        EXPECT_EQ(Status::TERMINATING, getStatus());
    };

    // init timers
    create(10.0, 0.01);

    // check status
    EXPECT_EQ(Status::STOPPED, getStatus());

    // run
    Loop::run();

    // check status
    EXPECT_EQ(Status::STOPPED, getStatus());

}


TEST_F(LoopTest, DefaultSteps) {

    // create generic component
    auto gen = createGenericComponent();

    // init function
    gen->initFnc = [&gen] (double t) {
        EXPECT_EQ(0, gen->getSteps());
    };

    // step function
    gen->stepFnc = [gen] (double t) {
        auto steps = (unsigned int) (t * 100 + 1.001);
        EXPECT_EQ(steps, gen->getSteps());
    };

    // termination function
    gen->termFnc = [&gen] (double t) {
        EXPECT_EQ(1001, gen->getSteps());
    };

    // init timers
    run(10.0, 0.01);

}


TEST_F(LoopTest, Steps) {

    // create generic component
    auto gen = createGenericComponent();
    gen->setTimeStepSize(0.1, 2.0);

    // init function
    gen->initFnc = [&gen] (double t) {
        EXPECT_EQ(0, gen->getSteps());
    };

    // step function
    gen->stepFnc = [gen] (double t) {
        auto step = (unsigned int) ((t - 2.0) * 10 + 1.001);
        EXPECT_EQ(step, gen->getSteps());
    };

    // termination function
    gen->termFnc = [&gen] (double t) {
        EXPECT_EQ(81, gen->getSteps());
    };

    // create and run
    run(10.0, 0.01);

}


TEST_F(LoopTest, DifferentSteps) {

    // create generic component
    auto gen = createGenericComponent();

    // setup timing
    gen->setTimeStepSize(0.3, 2.3);

    // init function
    gen->initFnc = [&gen] (double t) {
        EXPECT_EQ(0, gen->getSteps());
    };

    // step function
    gen->stepFnc = [gen] (double t) {
        auto step = (unsigned int) ((t - 2.3) / 0.3 + 1.001);
        EXPECT_EQ(step, gen->getSteps());
    };

    // termination function
    gen->termFnc = [&gen] (double t) {
        EXPECT_EQ(26, gen->getSteps());
    };

    // create and run
    run(10.0, 0.01);

}
