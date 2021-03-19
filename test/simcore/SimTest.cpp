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

#include <simcore/IComponent.h>
#include <simcore/Loop.h>
#include <simcore/timers/BasicTimer.h>
#include <simcore/timers/TimeIsUp.h>
#include <simcore/timers/RealTimeTimer.h>
#include <gtest/gtest.h>


class SimTest : public ::testing::Test, public sim::IComponent {


public:

    double cumulatedTime = 0.0;


    SimTest() = default;
    ~SimTest() override = default;

    void initialize(double initTime) override {}
    void terminate(double simTime) override {}

    void step(double t, double dt) override {

        // cumulate time
        cumulatedTime += dt;

    }


};


TEST(SimTestBasic, SimpleProcess) {

    using namespace ::sim;

    // create objects
    BasicTimer timer;
    TimeIsUp stop;

    // set parameters
    timer.setTimeStepSize(1.0);
    stop.setStopTime(10.0);

    // create loop
    Loop sim;

    // set timer and stop condition
    sim.setTimer(&timer);
    sim.addStopCondition(&stop);

    // models
    sim.addComponent(&stop);

    // initialize simulation
    EXPECT_NO_THROW(sim.run());
    EXPECT_EQ(IStopCondition::StopCode::SIM_ENDED, stop.getCode());

}


TEST_F(SimTest, Model) {

    using namespace ::sim;

    // create objects
    RealTimeTimer timer;
    TimeIsUp stop;

    // set timer parameters
    timer.setTimeStepSize(0.01);
    timer.setAcceleration(10.0);

    // set stop time
    stop.setStopTime(10.0);

    // create loop
    Loop sim;

    // set timer and stop condition
    sim.setTimer(&timer);
    sim.addStopCondition(&stop);

    // models
    sim.addComponent(&stop);
    sim.addComponent(this);

    // check status
    EXPECT_EQ(Loop::Status::STOPPED, sim.getStatus());

    // run simulation
    sim.run();

    // check time and steps
    EXPECT_NEAR(10.0, timer.time(), 1e-8);
    EXPECT_NEAR(0.0, this->_initTime, 1e-8);
    EXPECT_NEAR(10.0, cumulatedTime, 1e-8);
    EXPECT_NEAR(10.0, _termTime, 1e-8);
    EXPECT_EQ(1001, this->_steps);

    // check status
    EXPECT_EQ(Loop::Status::STOPPED, sim.getStatus());

}