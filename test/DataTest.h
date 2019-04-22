//
// Created by klimke on 16.04.2019.
//

#ifndef SIMCORE_DATATEST_H
#define SIMCORE_DATATEST_H

#include <gtest/gtest.h>
#include <core/Model.h>
#include <core/Loop.h>
#include <core/IStopCondition.h>
#include <components/timers/BasicTimer.h>
#include <components/timers/TimeIsUp.h>
#include <components/data/TimeReporter.h>

class DataTest : public ::testing::Test, public sim::Model {

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

private:

    Parameters _param{};
    Input      _input{};
    State      _state{};



protected:

    // create objects
    BasicTimer timer;
    TimeIsUp stop;
    ::sim::Loop loop;

    double time  = 0.0;


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

        auto dt = IComponent::timeStep(simTime);

        // write data into variable
        time = simTime;

        // save time step size to state.sa
        _state.sa = dt;

        return true;

    }


public:

    void initialize(double initTime) override {

        IComponent::initializeTimer(initTime);

        _state.sa = 1.0;
        _state.sb = 1.0;
        _input.ia = 2.0;
        _input.ib = 3.0;
        _param.pa = 4.0;
        _param.pb = 5.0;

        time = initTime;

    }

    void terminate(double simTime) override {

        time = simTime;
        _state.sb = simTime;

    }


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

    // accessors
    PARAM_ACCESS(param)
    INPUT_ACCESS(input)
    STATE_ACCESS(state)

};


TEST_F(DataTest, ReportTime) {

    // create out-stream
    std::stringstream ostr;

    // create synchronized model (in this case time reporter)
    TimeReporter timeRep;
    loop.addComponent(&timeRep);

    // setup for time reporter
    timeRep.setTimeStepSize(5.0, 1.0);
    timeRep.ostream(ostr);

    // initialize simulation
    loop.run();

    // check time and status
    EXPECT_NEAR(10.0, timer.time(), 1e-8);

    // check output of timer
    EXPECT_EQ("1s\n6s\n", ostr.str());

}



TEST_F(DataTest, DataManager) {

    using namespace ::sim;
    using namespace ::sim::data;

    // register model to data manager
    DataManager data;
    data.registerValues("Test", *this);

    // add this to loop
    loop.addComponent(this);

    // initialize simulation
    loop.run();

    // check data in data manager
    EXPECT_NEAR(0.1,  *((double*) data.getValue("Test.state.sa")),     1e-9);
    EXPECT_NEAR(10.0, *((double*) data.getValue("Test.state.sb")),     1e-9);
    EXPECT_NEAR(2.0,  *((double*) data.getValue("Test.input.ia")),     1e-9);
    EXPECT_NEAR(3.0,  *((double*) data.getValue("Test.input.ib")),     1e-9);
    EXPECT_NEAR(4.0,  *((double*) data.getValue("Test.parameter.pa")), 1e-9);
    EXPECT_NEAR(5.0,  *((double*) data.getValue("Test.parameter.pb")), 1e-9);

    EXPECT_THROW(data.getValue("another_value"), std::exception);

}



#endif //SIMCORE_DATATEST_H
