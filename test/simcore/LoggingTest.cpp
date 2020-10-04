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
#include <sstream>
#include <simcore/Loop.h>
#include <simcore/ISynchronized.h>
#include <simcore/timers/BasicTimer.h>
#include <simcore/timers/TimeIsUp.h>
#include <simcore/logging/FileLogger.h>
#include <simcore/logging/TimeReporter.h>
#include <simcore/utils/sim_functions.h>
#include <gtest/gtest.h>


class LoggingTest : public ::testing::Test, public sim::ISynchronized {

public:

    double time{};
    double deltaTime{};
    double value1{};
    double value2{};

    // create objects
    ::sim::BasicTimer timer{};
    ::sim::TimeIsUp stop{};
    ::sim::Loop loop{};

    void SetUp() override {

        // set parameters
        timer.setTimeStepSize(0.01);
        stop.setStopTime(10.0);

        // set timer and stop condition
        loop.setTimer(&timer);
        loop.addStopCondition(&stop);

        // models
        loop.addComponent(&stop);
        loop.addComponent(this);

        // set time step size
        setTimeStepSize(0.01);

    }


    void step(double simTime, double timeStepSize) override {

        // timing
        time = simTime;
        deltaTime = timeStepSize;

        // some random values
        value1 = cos(time * 2.0);
        value2 = sin(time * 3.0);

    }


protected:

    void initialize(double t) override {}
    void terminate(double t) override {}

};


TEST_F(LoggingTest, SaveDataCSV) {

    using namespace sim::logging;

    // file logger
    FileLogger logger{};
    logger.setTimeStepSize(0.01);
    loop.addComponent(&logger);

    // set data file
    auto dataFile = "data.csv";

    // create logger
    logger.setFilename(dataFile);

    // register values
    logger.registerValue("t",  &time);
    logger.registerValue("dt", &deltaTime);
    logger.registerValue("a",  &value1);
    logger.registerValue("b",  &value2);

    // set format
    logger.setLogFormat(FileLogger::LogFormat::CSV);

    // run simulation
    loop.run();

    // open file
    std::ifstream file(dataFile);
    std::string line;

    // check first line
    getline(file, line);
    EXPECT_EQ(R"("t","dt","a","b")", line);

    // check second line
    getline(file, line);
    EXPECT_EQ("0,0,1,0", line); // TODO: check with near

}


TEST_F(LoggingTest, TimeReporter) {

    using namespace sim::logging;

    // stream
    std::stringstream ss;

    // time reporter
    TimeReporter rep{};
    rep.ostream(ss);
    rep.setTimeStepSize(1.0);
    loop.addComponent(&rep);

    // run simulation
    loop.run();



}

#pragma clang diagnostic pop