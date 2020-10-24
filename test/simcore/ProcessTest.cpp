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
// Created by Jens Klimke on 2020-08-10.
//

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <nlohmann/json.hpp>
#include <simcore/testing/SimulationModel.h>
#include <gtest/gtest.h>


class ProcessTest : public ::testing::Test, public sim::testing::SimulationModel {
protected:

    void SetUp() override {

        // create simulation
        create(10.0, 0.1);

    }


};


TEST_F(ProcessTest, Errors) {

    // exception
    EXPECT_THROW(addStopCondition(_stopConditions.front()), sim::SetupException);
    EXPECT_THROW(addComponent(this), sim::SetupException);
    EXPECT_THROW(abort(), sim::ProcessException);
    EXPECT_THROW(_execute(), sim::ProcessException);
    EXPECT_THROW(_executeDirectly(), sim::ProcessException);

    // create test loop
    Loop loop;
    EXPECT_THROW(loop.run(), sim::SetupException);

    setCallback([this](const sim::testing::TimeStep& step){

        // only check step 10
        if(step.steps != 10)
            return;

        // try to initialize
        EXPECT_THROW(_initialize(), sim::ProcessException);

    });


    EXPECT_NO_THROW(run());

}


TEST_F(ProcessTest, Status) {

    EXPECT_EQ(Status::STOPPED, getStatus());

    // initialize and check
    _initialize();
    EXPECT_EQ(Status::INITIALIZED, getStatus());

    // set callback
    setCallback([this](const sim::testing::TimeStep &) {
        EXPECT_EQ(Status::RUNNING, getStatus());
    });

    // execute and check
    _execute();
    EXPECT_EQ(Status::RUNNING, getStatus());

    // terminate and check
    _terminate();
    EXPECT_EQ(Status::STOPPED, getStatus());

}


TEST_F(ProcessTest, StoreAndResume) {

    nlohmann::json json{{"loop", {}}, {"timer", {}}, {"stop", {}}, {"model", {}}};

    setCallback([this, &json](const sim::testing::TimeStep &step) {

        // ignore all steps, except step 50
        if(step.steps != 50)
            return;

//        // loop and timer
//        sim::Loop::toJSON(json["loop"]);
//        _timer->toJSON(json["timer"]);
//        _stopConditions.front()->toJSON(json["stop"]);
//        sim::ISynchronized::toJSON(json["model"]);

        // abort
        abort();

    });

    // run
    run();

    // TODO: Dump by external functions
    std::cout << json << std::endl;

    // setup and re-run
    sim::Loop loop{};
    sim::BasicTimer timer{};
    sim::TimeIsUp stop{};

    // structure
    loop.setTimer(&timer);
    loop.addStopCondition(&stop);
    loop.addComponent(&stop);
    loop.addComponent(this);

//    // get from json
//    loop.fromJSON(json["loop"]);
//    stop.fromJSON(json["stop"]);
//    timer.fromJSON(json["timer"]);

    // run loop
//     loop.run();

    EXPECT_FALSE(true);

}



#pragma clang diagnostic pop