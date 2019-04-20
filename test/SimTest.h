//
// Created by Jens Klimke on 2019-03-19.
//
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cert-err58-cpp"
#pragma ide diagnostic ignored "cppcoreguidelines-avoid-goto"

#ifndef SIMCORE_PLUGINTEST_H
#define SIMCORE_PLUGINTEST_H

#include <gtest/gtest.h>
#include <core/IComponent.h>
#include <core/Loop.h>
#include <core/IParameterizable.h>
#include <core/IStorable.h>
#include <core/DataManager.h>
#include <components/timers/BasicTimer.h>
#include <components/timers/TimeIsUp.h>



class SimTest : public ::testing::Test, public sim::IComponent, public sim::IParameterizable, public sim::IStorable {


public:

    struct Parameters {
        double pa;
        double pb;
    };

    struct Input {
        double ia;
        double ib;
    };

    struct State {
        double sa;
        double sb;
    };

    bool wasInitialized = false;
    int steps = 0;
    bool wasTerminated = false;


private:

    Parameters _param{};
    Input      _input{};
    State      _state{};


public:


    SimTest() = default;


    ~SimTest() override = default;


    void initialize(double initTime) override {

        IComponent::initializeTimer(initTime);
        wasInitialized = true;

        _state.sa = 1.0;
        _state.sb = 1.0;
        _input.ia = 2.0;
        _input.ib = 3.0;
        _param.pa = 4.0;
        _param.pb = 5.0;

    }


    bool step(double simTime) override {

        auto dt = IComponent::timeStep(simTime);
        steps++;

        _state.sa = dt;

        return true;

    }


    void terminate(double simTime) override {

        wasTerminated = true;
        _state.sb = simTime;

    }

    // accessors
    PARAM_ACCESS(param)
    INPUT_ACCESS(input)
    STATE_ACCESS(state)


    std::vector<DataEntry> getData(Context context) override {

        std::vector<DataEntry> ret;
        ret.reserve(2);

        switch(context) {
            case Context::PARAMETER:
                ADD(ret, pa, _param)
                ADD(ret, pb, _param)
                break;
            case Context::INPUT:
                ADD(ret, ia, _input)
                ADD(ret, ib, _input)
                break;
            case Context::STATE:
                ADD(ret, sa, _state)
                ADD(ret, sb, _state)
                break;
            default:
                break;
        }

        return ret;

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
    sim.addModel(&stop);

    // initialize simulation
    EXPECT_NO_THROW(sim.run());
    EXPECT_EQ(IStopCondition::StopCode::SIM_ENDED, stop.getCode());

}


TEST_F(SimTest, DataHandling) {

    using namespace ::sim;

    // create objects
    BasicTimer timer;
    TimeIsUp stop;

    // set parameters
    timer.setTimeStepSize(0.1);
    stop.setStopTime(10.0);

    // create loop
    Loop sim;
    DataManager data;

    // set timer and stop condition
    sim.setTimer(&timer);
    sim.addStopCondition(&stop);

    // models
    sim.addModel(&stop);
    sim.addModel(this);

    // register model to data manager
    data.registerValues("Test", *this);

    // check status
    EXPECT_EQ(Loop::Status::STOPPED, sim.getStatus());

    // initialize simulation
    sim.run();

    // check time and status
    EXPECT_NEAR(10.0, timer.time(), 1e-8);
    EXPECT_EQ(Loop::Status::STOPPED, sim.getStatus());

    // check values
    EXPECT_TRUE(this->wasInitialized);
    EXPECT_TRUE(this->wasTerminated);
    EXPECT_EQ(101, this->steps);

    // check data in data manager
    EXPECT_NEAR(0.1,  *((double*) data.getValue("Test.state.sa")),     1e-9);
    EXPECT_NEAR(10.0, *((double*) data.getValue("Test.state.sb")),     1e-9);
    EXPECT_NEAR(2.0,  *((double*) data.getValue("Test.input.ia")),     1e-9);
    EXPECT_NEAR(3.0,  *((double*) data.getValue("Test.input.ib")),     1e-9);
    EXPECT_NEAR(4.0,  *((double*) data.getValue("Test.parameter.pa")), 1e-9);
    EXPECT_NEAR(5.0,  *((double*) data.getValue("Test.parameter.pb")), 1e-9);

    EXPECT_THROW(data.getValue("another_value"), std::exception);

}



#endif //SIMCORE_PLUGINTEST_H

#pragma clang diagnostic pop