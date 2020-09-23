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
#include <yaml-cpp/yaml.h>
#include <simcore/timers/RealTimeTimer.h>
#include <simcore/timers/TimeIsUp.h>
#include "Simulation.h"
#include "SampleApplication.h"

class ApplicationTest : public ::testing::Test, protected SampleApplication, protected Simulation {

protected:

    sim::ITimer *_timer = nullptr;


public:

    void SetUp() override {

        // get config file path
        std::string path = CONFIG_DIR;
        path += "/config.yaml";

        // load config
        YAML::Node config = YAML::LoadFile(path);

        // get timer type
        auto type = config["timer"]["type"].as<std::string>();

        // timer
        if(type == "RealTimeTimer") {

            // get acceleration
            auto acc = config["timer"]["acceleration"].as<double>();
            auto dt = config["timer"]["timeStepSize"].as<double>();

            // create timer
            auto timer = new sim::RealTimeTimer;

            // setup
            timer->setAcceleration(acc);
            timer->setTimeStepSize(dt);

            // set timer
            this->setTimer(timer);

        }

        // has stop conditions
        auto scs = config["stopConditions"];
        for(const auto &sc : scs) {

            if(sc["type"].as<std::string>() == "TimeIsUp") {

                // create time is up stop condition
                auto tiu = new sim::TimeIsUp;

                // parameters
                tiu->setStopTime(sc["endTime"].as<double>());

                // register
                this->addStopCondition(tiu);
                this->addComponent(tiu);

            }

        }

    }


    void TearDown() override {

        delete _timer;

    }

};


TEST_F(ApplicationTest, Create) {

    // run simulation
    this->run();

    // init and termination time
    EXPECT_DOUBLE_EQ(0.0, this->initTime);
    EXPECT_DOUBLE_EQ(10.0, this->termTime);

}

