#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cert-err58-cpp"
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
// Created by Jens Klimke on 2020-09-10.
//

#include <cmath>
#include <simcore/Loop.h>
#include <simcore/ISynchronized.h>
#include <simcore/logging/TimeReporter.h>
#include <simcore/timers/RealTimeTimer.h>
#include <simcore/timers/TimeIsUp.h>
#include <simcore/logging/UDPLogger.h>
#include <simcore/logging/WebsocketLogger.h>
#include <simcore/utils/functions.h>
#include <gtest/gtest.h>


class RemoteLoggingTest : public ::testing::Test, public sim::ISynchronized {

public:

    double time{};
    double deltaTime{};

    // create objects
    sim::logging::TimeReporter rep{};
    sim::RealTimeTimer timer{};
    sim::TimeIsUp stop{};
    sim::Loop loop{};

    void SetUp() override {

        // set parameters
        timer.setTimeStepSize(0.1);
        timer.setAcceleration(10.0);
        stop.setStopTime(10.0);
        rep.setTimeStepSize(1.0);
        setTimeStepSize(1.0);

        // set timer and stop condition
        loop.setTimer(&timer);
        loop.addStopCondition(&stop);

        // models
        loop.addComponent(&stop);
        loop.addComponent(&rep);
        loop.addComponent(this);

    }


    void step(double simTime, double timeStepSize) override {

        // timing
        time = simTime;
        deltaTime = timeStepSize;

    }


protected:

    void initialize(double t) override {}
    void terminate(double t) override {}

};


TEST_F(RemoteLoggingTest, SendUDP) {

    using namespace sim::logging;

    // file logger
    UDPLogger logger{};
    logger.setTimeStepSize(1.0);
    logger.setHostAndPort("localhost", "3003");

    // add component
    loop.addComponent(&logger);

    // register values
    logger.registerValue("t",  &time);
    logger.registerValue("dt", &deltaTime);

    // run simulation
    loop.run();


}



TEST_F(RemoteLoggingTest, SendWebsocket) {

    using namespace sim::logging;

    // file logger
    WebsocketLogger logger{};
    logger.setTimeStepSize(1.0);
    logger.setHostPortAndPath("localhost", "1880", "/ws/simcore");

    // add component
    loop.addComponent(&logger);

    // register values
    logger.registerValue("t",  &time);
    logger.registerValue("dt", &deltaTime);

    // run simulation
    loop.run();


}


#pragma clang diagnostic pop