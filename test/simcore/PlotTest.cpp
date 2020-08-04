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
// Created by Jens Klimke on 2020-03-08.
//

#include <simcore/Loop.h>
#include <simcore/ISynchronized.h>
#include <simcore/timers/BasicTimer.h>
#include <simcore/timers/TimeIsUp.h>
#include <simcore/data/PlotLogger.h>
#include <simcore/data/JsonFileReporter.h>
#include <simcore/functions.h>
#include <gtest/gtest.h>
#include <cmath>


class PlotTest : public ::testing::Test, public sim::ISynchronized {

public:

    double time;
    double value1;
    double value2;

    // create objects
    BasicTimer timer;
    TimeIsUp stop;
    ::sim::Loop loop;

    PlotLogger plotLogger;
    JsonFileReporter jsonLogger;

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

        // add loggers
        loop.addComponent(&jsonLogger);
        loop.addComponent(&plotLogger);

    }


    bool step(double simTime) override {

        // timing
        auto dt = IComponent::timeStep(simTime);
        time = simTime;

        // some random values
        value1 = cos(time * 2.0);
        value2 = sin(time * 3.0);

        return true;

    }


    void terminate(double simTime) override {



    }


};


TEST_F(PlotTest, SaveData) {

    // set data file
    auto dataFile = "data.json";
    auto plotFile = sim::fnc::string_format("%s/plot.json", LOG_DIR);

    // data pairs
    std::vector<std::pair<std::string, std::string>> dataPair =
            {{"Value 1", "a"},
             {"Value 2", "b"}};

    // create logger
    jsonLogger.setFilename(dataFile);
    jsonLogger.addValue("a", &value1);
    jsonLogger.addValue("b", &value2);

    // create plot definition
    plotLogger.create(plotFile, "Plot Test");
    plotLogger.setDataFile(dataFile);
    plotLogger.addFigure("first", "Value 1 over time", "Time t [s]", "Value v [-]", "time", "a", true);
    plotLogger.addFigure("second", "Value 1 and 2 over time", "Time t [s]", "Value v [-]", "time", dataPair);

    // run loop
    loop.run();

}
