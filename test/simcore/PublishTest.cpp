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
// Created by Jens Klimke on 2019-04-22.
//

#include <simcore/Loop.h>
#include <simcore/Model.h>
#include <simcore/timers/RealTimeTimer.h>
#include <simcore/timers/TimeIsUp.h>
#include <simcore/socket/DataPublisher.h>
#include <simcore/functions.h>
#include <gtest/gtest.h>



class DataContainer : public sim::data::IStorable {

    double velocity = 0.0;
    double acceleration = 0.0;


public:

    DataContainer() = default;
    virtual ~DataContainer() = default;


    void step(double dt) {

        // a model which accelerates to reach a velocity of 10.0
        acceleration = 1.0 - std::pow(velocity / 10.0, 4.0);
        velocity += acceleration * dt;

    }

    std::vector<DataEntry> getData(Context context) const override {

        // create vector
        std::vector<DataEntry> vector{};

        // add values
        if(context == Context::STATE) {
            vector.emplace_back(sim::data::createDataEntry("velocity", &velocity));
            vector.emplace_back(sim::data::createDataEntry("acceleration", &acceleration));
        }

        // return vector
        return vector;

    }


};



class PublishTest : public ::testing::Test, public sim::data::DataPublisher, public sim::data::DataManager {


protected:

    // create objects
    RealTimeTimer timer;
    TimeIsUp stop;
    ::sim::Loop loop;

    // data container
    DataContainer container;

    void SetUp() override {

        // set parameters
        timer.setTimeStepSize(1.0);
        timer.setAcceleration(1.0);
        stop.setStopTime(30.0);

        // set timer and stop condition
        loop.setTimer(&timer);
        loop.addStopCondition(&stop);

        // models (stop condition and data publisher)
        loop.addComponent(&stop);
        loop.addComponent(this);

        // setup data publisher
        this->setHost("raspberrypi.local", "1880");
        this->setPath("/ws/chat");

        // add container to data manager and add data manager
        this->registerStorable("container", container);
        this->setDataManager(this);

    }


private:


    void initialize(double initTime) override {

        // initialize data publisher
        DataPublisher::initialize(initTime);

        // initialize timer
        initializeTimer(initTime);

    }

    bool step(double simTime) override {

        // get time step size
        auto dt = timeStep(simTime);

        // run publisher step
        DataPublisher::step(simTime);

        // run the container step
        container.step(dt);

        return true;

    }


};


TEST_F(PublishTest, Publisher) {

    // TODO: how to receive data in tests -> write parallel programme?

//    // initialize simulation
//    EXPECT_NO_THROW(loop.run());
//
//    // get data from container
//    auto state = container.getData(sim::data::IStorable::Context::STATE);
//
//    auto v = *((double*) state[0].data->v());
//    auto a = *((double*) state[1].data->v());
//
//    EXPECT_NEAR(10.0, v, 1e-3);
//    EXPECT_NEAR( 0.0, a, 1e-3);

}
