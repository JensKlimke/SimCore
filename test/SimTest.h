//
// Created by Jens Klimke on 2019-03-19.
//
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cert-err58-cpp"
#pragma ide diagnostic ignored "cppcoreguidelines-avoid-goto"

#ifndef SIMULATION_FRAMEWORK_PLUGINTEST_H
#define SIMULATION_FRAMEWORK_PLUGINTEST_H

#include <gtest/gtest.h>
#include <core/IModel.h>
#include <core/Loop.h>
#include <core/IParameterizable.h>
#include <core/IStorable.h>
#include <core/DataManager.h>
#include <models/timers/BasicTimer.h>
#include <models/timers/TimeIsUp.h>
#include <models/reporters/TimeReporter.h>


/**
 * TODO:
 * * uncomment first test, and check problems
 * * Create model to test stop condition with objectives (Objectives reached/missed)
 */
class SimTest : public ::testing::Test, public sim::IModel, public sim::IParameterizable, public sim::IStorable {


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
    double dt = INFINITY;
    bool wasTerminated = false;


private:

    Parameters _param{};
    Input      _input{};
    State      _state{};


public:


    SimTest() = default;


    ~SimTest() override = default;


    void initialize(double initTime) override {

        IModel::initializeTimer(initTime);
        wasInitialized = true;

        _state.sa = 1.0;
        _state.sb = 1.0;
        _input.ia = 2.0;
        _input.ib = 3.0;
        _param.pa = 4.0;
        _param.pb = 5.0;

    }


    bool step(double simTime) override {

        dt = IModel::timeStep(simTime);
        steps++;

        _state.sa = dt;

        return true;

    }


    void terminate(double simTime) override {

        wasTerminated = true;
        _state.sb = simTime;

    }


    bool getParameters(void **param) override {

        try {
            *param = &_param;
        } catch(...) {
            return false;
        }

        return true;
    }


    bool getInput(void **input) override {

        try {
            *input = &_input;
        } catch(...) {
            return false;
        }

        return true;

    }


    bool getState(void **state) override {

        try {
            *state = &_state;
        } catch(...) {
            return false;
        }

        return true;

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

};


TEST_F(SimTest, SimProcess) {

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


TEST_F(SimTest, Synchronized) {

    using namespace ::sim;

    // create objects
    BasicTimer timer;
    TimeIsUp stop;
    Loop sim;

    // set parameters
    timer.setTimeStepSize(0.1);

    // set parameters
    stop.setStopTime(10.0);

    // set timer
    sim.setTimer(&timer);

    // set stop condition
    sim.addStopCondition(&stop);
    sim.addModel(&stop);

    // create out-stream
    std::stringstream ostr{};

    // create synchronized model (in this case time reporter)
    TimeReporter timeRep;
    sim.addModel(&timeRep);

    // setup for time reporter
    timeRep.setTimeStepSize(5.0, 1.0);
    timeRep.ostream(ostr);

    // add another time reporter (for output in console)
    TimeReporter timeRepConsole;
    sim.addModel(&timeRepConsole);

    // setup for time reporter
    timeRepConsole.setTimeStepSize(1.0);

    // check status
    EXPECT_EQ(Loop::Status::STOPPED, sim.getStatus());

    // initialize simulation
    sim.run();

    // check time and status
    EXPECT_NEAR(10.0, timer.time(), 1e-8);
    EXPECT_EQ(Loop::Status::STOPPED, sim.getStatus());

    // check output of timer
    EXPECT_EQ("1s\n6s\n", ostr.str());

}


#endif //SIMULATION_FRAMEWORK_PLUGINTEST_H

#pragma clang diagnostic pop