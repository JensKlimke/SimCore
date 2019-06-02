//
// Created by Jens Klimke on 2019-04-22.
//

#ifndef SIMCORE_PUBLISHTEST_H
#define SIMCORE_PUBLISHTEST_H

#include <gtest/gtest.h>
#include <core/Model.h>
#include <components/timers/RealTimeTimer.h>
#include <components/data/DataPublisher.h>

class PublishTest : public ::testing::Test, public sim::IComponent {

public:

    double time  = 0.0;


protected:

    // create objects
    RealTimeTimer timer;
    TimeIsUp stop;
    ::sim::Loop loop;


    void SetUp() override {

        // set parameters
        timer.setTimeStepSize(0.1);
        stop.setStopTime(10.0);

        // set timer and stop condition
        loop.setTimer(&timer);
        loop.addStopCondition(&stop);

        // models
        loop.addComponent(&stop);

    }


private:

    bool step(double simTime) override {

        // write data into variable
        time = simTime;
        return true;

    }


public:

    void initialize(double initTime) override {

        time = initTime;

    }

    void terminate(double simTime) override {

        time = simTime;

    }


};



TEST_F(PublishTest, Publisher) {

    // create out-stream
    std::stringstream ostr;

    // register this (this updates the time variable)
    loop.addComponent(this);

    // add publisher
    DataPublisher pub;
    pub.registerValue("time", &time);
    pub.setTimeStepSize(1.0);

    // add publisher to look
    loop.addComponent(&pub);

    // initialize simulation
    EXPECT_NO_THROW(loop.run());

}

#endif //SIMCORE_PUBLISHTEST_H
