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
                ADD(ret, pa, _param);
                ADD(ret, pb, _param);
                break;
            case Context::INPUT:
                ADD(ret, ia, _input);
                ADD(ret, ib, _input);
                break;
            case Context::STATE:
                ADD(ret, sa, _state);
                ADD(ret, sb, _state);
                break;
            default:
                break;
        }

        return ret;

    }

};



/*TEST_F(SimTest, TimerPlugin) {


    // load library
    void* handle = dlopen((std::string("plugins/libtest_plugin.") + PLUGIN_EXTENSION).c_str(), RTLD_LAZY);
    ASSERT_NE(nullptr, handle);


    // create function pointer to create function
    TestPlugin* (*create)();
    void (*destroy)(TestPlugin*);
    create = (TestPlugin* (*)())dlsym(handle, "create");
    destroy = (void (*)(TestPlugin*))dlsym(handle, "destroy");

    // check if function pointers exist
    ASSERT_NE(nullptr, create);
    ASSERT_NE(nullptr, destroy);

    // create timer
    TestPlugin* timer = create();
    ASSERT_NE(nullptr, timer);

    // start the timer
    timer->start();
    EXPECT_DOUBLE_EQ(0.0, timer->time());

    // set parameters
    TestPlugin::Parameters p{0.1};
    timer->setParameters(&p);
    timer->step();
    EXPECT_DOUBLE_EQ(0.1, timer->time());

    // timer step
    timer->step();
    EXPECT_DOUBLE_EQ(0.2, timer->time());


    // create a second one

    // create timer
    TestPlugin* timer2 = create();
    ASSERT_NE(nullptr, timer2);

    // start timer and set parameters
    timer2->start();
    timer2->setParameters(&p);
    timer2->step();
    EXPECT_DOUBLE_EQ(0.1, timer2->time());

    // timer step
    timer->step();
    EXPECT_DOUBLE_EQ(0.3, timer->time());


    destroy(timer);
    destroy(timer2);


}


TEST_F(SimTest, PluginManager) {


    ::sim::PluginManager pm;
    EXPECT_FALSE(pm.loadPlugin("na", "lib_that_doesnt_exist"));
    ASSERT_TRUE(pm.loadPlugin("basic_timer", std::string("plugins/libtest_plugin.") + PLUGIN_EXTENSION));

    // create instance
    auto timer = (TestPlugin *) pm.instance("basic_timer");
    ASSERT_NE(nullptr, timer);

    // start timer
    timer->start();
    EXPECT_DOUBLE_EQ(0.0, timer->time());

    // set parameters
    double *p;
    timer->getParameters((void **) &p);
    *p = 0.1;

    timer->step();
    EXPECT_DOUBLE_EQ(0.1, timer->time());

    // step timer
    timer->step();
    EXPECT_DOUBLE_EQ(0.2, timer->time());

}
    */


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
    EXPECT_NEAR(0.1,  *((double*) data.getValue("Test.state.sa")), 1e-9);
    EXPECT_NEAR(10.0, *((double*) data.getValue("Test.state.sb")), 1e-9);
    EXPECT_DOUBLE_EQ(2.0,  *((double*) data.getValue("Test.input.ia")));
    EXPECT_DOUBLE_EQ(3.0,  *((double*) data.getValue("Test.input.ib")));
    EXPECT_DOUBLE_EQ(4.0,  *((double*) data.getValue("Test.parameter.pa")));
    EXPECT_DOUBLE_EQ(5.0,  *((double*) data.getValue("Test.parameter.pb")));

    EXPECT_THROW(data.getValue("another_value"), std::exception);

}


TEST_F(SimTest, Models) {

    using namespace ::sim;

    // create objects
    BasicTimer timer;
    TimeIsUp stop;

    // set parameters
    timer.setTimeStepSize(0.1);

    // set parameters
    stop.setStopTime(10.0);

    // create loop
    Loop sim;

    // set timer
    sim.setTimer(&timer);

    // set stop condition
    sim.addStopCondition(&stop);
    sim.addModel(&stop);

    // check status
    EXPECT_EQ(Loop::Status::STOPPED, sim.getStatus());

    // initialize simulation
    sim.run();

    EXPECT_NEAR(10.0, timer.time(), 1e-8);
    EXPECT_EQ(Loop::Status::STOPPED, sim.getStatus());


}


#endif //SIMULATION_FRAMEWORK_PLUGINTEST_H

#pragma clang diagnostic pop