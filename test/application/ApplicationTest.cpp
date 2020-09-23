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
#include <simcore/framework/Simulation.h>
#include <simcore/framework/ModelManager.h>
#include "SampleApplication.h"

SampleApplication *create_new_sample_application(std::string &&name, double value, unsigned long n) {

    auto ptr = new SampleApplication();

    ptr->timeSteps.reserve(n);
    ptr->setup(name, value);

    return ptr;

}

sim::Model *create_new_sample_application_from_conf(const sim::framework::Config &conf) {

    return create_new_sample_application(
            conf["name"].as<std::string>(),
            conf["value"].as<double>(),
            conf["size"].as<unsigned long>()
    );

}

class ApplicationTest : public ::testing::Test, protected Simulation {

public:

    void SetUp() override {

        // register creator function
        sim::framework::ModelManager::registerModelType("SampleApplication", create_new_sample_application_from_conf);

    }


    void TearDown() override {}

};



TEST_F(ApplicationTest, Create) {

    typedef YAML::Node Config;

    // create configuration
    YAML::Node node{};
    node["test"]["name"] = "TestModel";
    node["test"]["value"] = 123.45;
    node["test"]["size"] = 101;
    node["timer"]["stopTime"] = 10.0;

    // create model
    sim::framework::ModelManager manager{};
    auto model = dynamic_cast<SampleApplication*>(manager.create("SampleApplication", node["test"]));
    auto timer = dynamic_cast<SampleApplication*>(manager.create("SampleApplication", node["test"]));

    // check configuration
    EXPECT_EQ("TestModel", model->option_name);
    EXPECT_EQ(123.45, model->option_value);


    // set timer
    this->createTimer(0.01);

    // set simulation time
    this->setSimulationTime(10.0);

    // register this application
    model->setTimeStepSize(0.1);
    this->component(model);

    // execute
    this->execute();

    // init and termination time
    EXPECT_NEAR(0.0, model->initTime, EPS_SIM_TIME);
    EXPECT_NEAR(10.0, model->termTime, EPS_SIM_TIME);

    // check no of saved time steps
    EXPECT_EQ(101, model->timeSteps.size());

    // check time steps
    unsigned long i = 0;
    for(auto &step : model->timeSteps) {

        // check time and time step size
        EXPECT_NEAR(0.1 * i, step.first, EPS_SIM_TIME);
        EXPECT_NEAR(i == 0 ? 0.0 : 0.1, step.second, EPS_SIM_TIME);

        // increment counter
        ++i;

    }

}

