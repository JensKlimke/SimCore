//
// Created by Jens Klimke on 2019-03-19.
//

#include <core/IComponent.h>
#include <core/Loop.h>
#include <components/data/DataManager.h>
#include <components/timers/BasicTimer.h>
#include <components/timers/TimeIsUp.h>
#include <components/timers/RealTimeTimer.h>
#include <gtest/gtest.h>


class SimTest : public ::testing::Test, public sim::IComponent {


public:

    double time = 0.0;
    double finalTime = 0.0;

    bool wasInitialized = false;
    int steps = 0;
    bool wasTerminated = false;


public:


    SimTest() = default;


    ~SimTest() override = default;


    void initialize(double initTime) override {

        IComponent::initializeTimer(initTime);

        time = initTime;
        steps = 0;

        wasInitialized = true;

    }


    bool step(double simTime) override {

        auto dt = IComponent::timeStep(simTime);

        time += dt;
        steps++;

        return true;

    }


    void terminate(double simTime) override {

        wasTerminated = true;
        finalTime = simTime;

    }

};


TEST(SimTestBasic, SimpleProcess) {

    using namespace ::sim;

    // create objects
    BasicTimer timer;
    TimeIsUp stop;

    // set parameters
    timer.setTimeStepSize(1.0);
    stop.setStopTime(10.0);

    // create loop
    Loop sim;

    // set timer and stop condition
    sim.setTimer(&timer);
    sim.addStopCondition(&stop);

    // models
    sim.addComponent(&stop);

    // initialize simulation
    EXPECT_NO_THROW(sim.run());
    EXPECT_EQ(IStopCondition::StopCode::SIM_ENDED, stop.getCode());

}


TEST_F(SimTest, Model) {

    using namespace ::sim;

    // create objects
    RealTimeTimer timer;
    TimeIsUp stop;

    // set parameters
    timer.setTimeStepSize(0.01);
    stop.setStopTime(1.0);

    // create loop
    Loop sim;
    data::DataManager data;

    // set timer and stop condition
    sim.setTimer(&timer);
    sim.addStopCondition(&stop);

    // models
    sim.addComponent(&stop);
    sim.addComponent(this);

    // check status
    EXPECT_EQ(Loop::Status::STOPPED, sim.getStatus());

    // run simulation, TODO: check time
    sim.run();

    // check time and steps
    EXPECT_NEAR(1.0, timer.time(), 1e-8);
    EXPECT_NEAR(1.0, time, 1e-8);
    EXPECT_NEAR(1.0, finalTime, 1e-8);
    EXPECT_EQ(101, this->steps);

    // check status
    EXPECT_EQ(Loop::Status::STOPPED, sim.getStatus());

    // check values
    EXPECT_TRUE(this->wasInitialized);
    EXPECT_TRUE(this->wasTerminated);

}
