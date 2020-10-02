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

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <gtest/gtest.h>
#include <simcore/ISynchronized.h>
#include <simcore/Loop.h>
#include <simcore/timers/TimeIsUp.h>
#include <simcore/timers/RealTimeTimer.h>
#include <simcore/logging/TimeReporter.h>
#include <simcore/testing/SimulationTest.h>


class SimulationTestTest : public ::testing::Test, public sim::testing::SimulationTest<sim::logging::TimeReporter> {


public:

    SimulationTestTest() = default;
    ~SimulationTestTest() override = default;


    void checkStepState() {

        // interrupt
        addPreCallback([this] () {

            // check term time (to be inf -> initialized)
            EXPECT_TRUE(std::isinf(time().termTime));

            // check time
            EXPECT_NEAR(1.0 + 3.0 * time().steps, time().time, EPS_SIM_TIME);

            // check time
            if(time().steps == 0)
                EXPECT_NEAR(0.0, time().deltaTime, EPS_SIM_TIME);
            else
                EXPECT_NEAR(3.0, time().deltaTime, EPS_SIM_TIME);

            // check state
            EXPECT_EQ(Loop::Status::RUNNING, this->getStatus());

        });

    }


    void checkFinalState() {

        // check time and steps
        EXPECT_NEAR(10.0, time().time, EPS_SIM_TIME);
        EXPECT_NEAR(11.0, time().termTime, EPS_SIM_TIME);
        EXPECT_EQ(4, time().steps);

        // check status
        EXPECT_EQ(Loop::Status::STOPPED, this->getStatus());

    }

};


TEST_F(SimulationTestTest, SimulationLifeCycle) {

    using namespace ::sim;

    // set check during running
    checkStepState();

    // setup
    create(11.0, 0.01, 1.0);

    // set time step size and stream for dummy component
    std::ostringstream ss;
    ostream(ss);
    setTimeStepSize(3.0);

    // run simulation
    run();

    // check final state
    checkFinalState();

    // check output
    EXPECT_EQ(R"(t=1s, dt=0s\nt=4s, dt=3s\nt=7s, dt=3s\nt=10s, dt=3s\n)", ss.str());

}


//TEST_F(SimTest, MultipleRuns) {
//
//    using namespace ::sim;
//
//    // set check during running
//    setPrePostChecks();
//
//    // create objects
//    BasicTimer timer;
//    TimeIsUp stop;
//
//    // set timer parameters
//    timer.setTimeStepSize(0.01);
//    timer.setStartTime(1.0);
//
//    // setup
//    setup(timer, stop);
//
//    // run ten times
//    for(unsigned int i = 0; i < 10; ++i) {
//
//        // run simulation
//        this->run();
//
//        // check final state
//        checkFinalState(timer, stop);
//
//    }
//
//}
//
//
//TEST_F(SimTest, RealTimeSimulation) {
//
//    using namespace ::sim;
//
//    // set check during running
//    setPrePostChecks();
//
//    // create objects
//    RealTimeTimer timer;
//    TimeIsUp stop;
//
//    // set timer parameters
//    timer.setAcceleration(100.0);
//    timer.setTimeStepSize(0.01);
//    timer.setStartTime(1.0);
//
//    // setup
//    setup(timer, stop);
//
//    // run simulation
//    this->run();
//
//    // check final state
//    checkFinalState(timer, stop);
//
//}
//
//
//TEST_F(SimTest, NotSetProperly) {
//
//    using namespace ::sim;
//
//    // erroneous calls before initialization
//    EXPECT_THROW(this->run(), ProcessException);
//    EXPECT_THROW(this->stop(), ProcessException);
//    EXPECT_THROW(this->_execute(), ProcessException);
//
//    // create objects
//    BasicTimer timer;
//    timer.setTimeStepSize(0.01);
//
//    // interrupt
//    pre = [this] (double t, double dt) {
//
//        // at t=0.1 s
//        if(t > 0.099999) {
//
//            // try to initialize during run
//            EXPECT_THROW(Loop::_initialize(), ProcessException);
//
//            // stop
//            stop();
//
//        }
//
//    };
//
//    // set timer and stop condition
//    this->setTimer(&timer);
//    this->addComponent(this);
//
//    // run simulation
//    this->run();
//
//}


#pragma clang diagnostic pop