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
#include <simcore/utils/functions.h>
#include <nlohmann/json.hpp>
#include <gtest/gtest.h>


class LoggingTest : public ::testing::Test, public sim::ISynchronized {

public:


    double time{};
    double deltaTime{};
    double value1{};
    double value2{};

    // create objects
    sim::logging::FileLogger logger{};
    sim::BasicTimer timer{};
    sim::TimeIsUp stop{};
    sim::Loop loop{};

    void SetUp() override {

        // set parameters
        timer.setTimeStepSize(0.125);
        stop.setStopTime(1.0);

        // set timer and stop condition
        loop.setTimer(&timer);
        loop.addStopCondition(&stop);

        // models
        loop.addComponent(&stop);
        loop.addComponent(this);

        // set time step size
        setTimeStepSize(0.0001);

        // file logger
        logger.setTimeStepSize(0.125);
        loop.addComponent(&logger);

        // register values
        logger.registerValue("t",  &time);
        logger.registerValue("dt", &deltaTime);
        logger.registerValue("a",  &value1);
        logger.registerValue("b",  &value2);

    }


    void step(double simTime, double timeStepSize) override {

        // timing
        time = simTime;
        deltaTime = timeStepSize;

        // some random values
        value1 = cos(time * M_PI * 2.0);
        value2 = sin(time * M_PI * 2.0);

    }


protected:

    void initialize(double t) override {}
    void terminate(double t) override {}

};


TEST_F(LoggingTest, SaveDataCSV) {

    using namespace sim::logging;

    // set filename and type
    auto dataFile = "data.csv";
    logger.setFilename(dataFile);
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
    EXPECT_EQ("0,0,1,0", line);

}


TEST_F(LoggingTest, SaveDataJSON) {

    using namespace sim::logging;

    // set filename and type
    auto dataFile = "data.json";
    logger.setFilename(dataFile);
    logger.setLogFormat(FileLogger::LogFormat::JSON);

    // run simulation
    loop.run();

    // open file
    std::ifstream file(dataFile);

    // read json file
    nlohmann::json json;
    file >> json;

    // iterate over content
    unsigned int i = 0;
    for(auto &obj : json) {

        EXPECT_DOUBLE_EQ(i * 0.125, obj["t"].get<double>());
        EXPECT_DOUBLE_EQ(i == 0 ? 0.0 : 0.125, obj["dt"].get<double>());
        EXPECT_NEAR(cos(M_PI * 0.25 * i), obj["a"].get<double>(), 1e-4);
        EXPECT_NEAR(sin(M_PI * 0.25 * i), obj["b"].get<double>(), 1e-4);

        ++i;

    }

}


#pragma clang diagnostic pop