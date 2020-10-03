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
#include <simcore/timers/SynchronizedTimer.h>
#include <simcore/timers/RealTimeTimer.h>
#include <simcore/logging/TimeReporter.h>
#include <gtest/gtest.h>


class SynchronizedTimerTest : public ::testing::Test, public sim::ISynchronized, protected sim::Loop {


public:

    double time = 0.0;
    double finalTime = 0.0;


    SynchronizedTimerTest() = default;
    ~SynchronizedTimerTest() override = default;


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

static bool stopSync = false;

void calcTime(sim::SynchronizedTimer *timer, bool realTime = false) {

    using namespace std::chrono;

    // start real time
    auto refTime = system_clock::now();

    // counter and stop flag
    unsigned long i = 0;

    // loop until
    while(!stopSync) {

        // wait
        std::this_thread::sleep_for (std::chrono::milliseconds (1));

        // calculate real time
        auto elapsed = duration_cast<milliseconds>(system_clock::now() - refTime);
        auto currTime = static_cast<double>(elapsed.count()) / 1000.0;

        // set reference time
        double time = realTime ? currTime : 0.1 * (double) i;
        timer->setReferenceTime(time);

        // increment i
        ++i;

    }

}


TEST_F(SynchronizedTimerTest, SyncTest) {

    using namespace ::sim;

    // unset stop flag
    stopSync = false;

    // create timer
    SynchronizedTimer timer;
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

    // run synchronizer
    std::thread timeThread(calcTime, &timer, false);

    // run loop
    this->run();

    // check
    EXPECT_NEAR(10.0, finalTime, 1e-3);

    // stop
    stopSync = true;

    // join thread
    timeThread.join();

}


TEST_F(SynchronizedTimerTest, SyncTestRT) {

    using namespace ::sim;

    // unset stop flag
    stopSync = false;

    // create timer
    SynchronizedTimer timer;
    timer.setAcceleration(100.0);
    timer.setTimeStepSize(0.1);

    // end of loop
    TimeIsUp stop;
    stop.setStopTime(10.0);

    // reporter
    sim::logging::TimeReporter rep;
    rep.setTimeStepSize(1.0);

    // set timer and add this as component
    this->setTimer(&timer);
    this->addComponent(this);
    this->addStopCondition(&stop);
    this->addComponent(&stop);
    this->addComponent(&rep);

    // run synchronizer
    std::thread timeThread(calcTime, &timer, true);

    // start real time
    auto start = std::chrono::system_clock::now();

    // run loop
    this->run();

    // create elapsed time
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start);
    auto runTime = static_cast<double>(elapsed.count()) / 1000.0;

    // check
    EXPECT_NEAR(10.0, finalTime, 1e-2);
    EXPECT_NEAR(0.1, runTime, 1e-2);

    // stop
    stopSync = true;

    // join thread
    timeThread.join();

}


TEST_F(SynchronizedTimerTest, RealTimeTimerTest) {

    using namespace ::sim;

    // create timer
    RealTimeTimer timer;
    timer.setAcceleration(100.0);
    timer.setTimeStepSize(0.1);

    // end of loop
    TimeIsUp stop;
    stop.setStopTime(10.0);

    // reporter
    sim::logging::TimeReporter rep;
    rep.setTimeStepSize(1.0);

    // set timer and add this as component
    this->setTimer(&timer);
    this->addComponent(this);
    this->addStopCondition(&stop);
    this->addComponent(&stop);
    this->addComponent(&rep);

    // run synchronizer
    std::thread timeThread(calcTime, &timer, true);

    // start real time
    auto start = std::chrono::system_clock::now();

    // run loop
    this->run();

    // create elapsed time
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start);
    auto runTime = static_cast<double>(elapsed.count()) / 1000.0;

    // check
    EXPECT_NEAR(10.0, finalTime, 1e-2);
    EXPECT_NEAR(0.1, runTime, 1e-2);

    // stop
    stopSync = true;

    // join thread
    timeThread.join();

}


#pragma clang diagnostic pop