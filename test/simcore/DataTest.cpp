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
// Created by Jens Klimke on 2019-04-16.
//

#include <simcore/Model.h>
#include <simcore/Loop.h>
#include <simcore/IStopCondition.h>
#include <simcore/IStorable.h>
#include <simcore/timers/BasicTimer.h>
#include <simcore/timers/TimeIsUp.h>
#include <simcore/data/TimeReporter.h>
#include <simcore/data/DataManager.h>
#include <gtest/gtest.h>
#include <map>


class DataTest : public ::testing::Test, public sim::Model, public sim::data::IStorable {

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


    double time = 0.0;
    std::string name;


protected:

    // create objects
    BasicTimer timer;
    TimeIsUp stop;
    ::sim::Loop loop;


    void SetUp() override {

        name = "DataTest";

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


    std::vector<DataEntry> getData(Context context) const override {

        std::vector<DataEntry> ret;
        ret.reserve(2);

        switch(context) {
            case Context::PARAMETER:
                ret.push_back(sim::data::createDataEntry("pa", &_param.pa));
                ret.push_back(sim::data::createDataEntry("pb", &_param.pb));
                ret.emplace_back(sim::data::createDataEntry("name",  &name));
                break;
            case Context::INPUT:
                ADD(ret, ia, _input);
                ADD(ret, ib, _input);
                break;
            case Context::STATE:
                ADD(ret, sa, _state);
                ADD(ret, sb, _state);
                ret.emplace_back(sim::data::createDataEntry("time", &time));
                break;
            default:
                break;
        }

        return ret;

    }


    void _data() {



    }


    // accessors
    PARAM_ACCESS(Parameters, _param)
    INPUT_ACCESS(Input, _input)
    STATE_ACCESS(State, _state)

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
    data.registerStorable("Test", *this);

    // add this to loop
    loop.addComponent(this);

    // initialize simulation
    loop.run();

    // check data in data manager
    EXPECT_NEAR(0.1,  *((double*) data.getValue("Test.state.sa")),     1e-9);
    EXPECT_NEAR(10.0, *((double*) data.getValue("Test.state.sb")),     1e-9);
    EXPECT_NEAR(10.0, *((double*) data.getValue("Test.state.time")),   1e-9);
    EXPECT_NEAR(2.0,  *((double*) data.getValue("Test.input.ia")),     1e-9);
    EXPECT_NEAR(3.0,  *((double*) data.getValue("Test.input.ib")),     1e-9);
    EXPECT_NEAR(4.0,  *((double*) data.getValue("Test.parameter.pa")), 1e-9);
    EXPECT_NEAR(5.0,  *((double*) data.getValue("Test.parameter.pb")), 1e-9);
    EXPECT_EQ("DataTest",  *((std::string*) data.getValue("Test.parameter.name")));

    std::string json(R"({"Test":{"parameter":{"pa":4,"pb":5,"name":"DataTest"},"input":{"ia":2,"ib":3},"state":{"sa":0.1,"sb":10,"time":10}}})");
    std::stringstream ss;
    data.streamTo(ss);

    EXPECT_EQ(json, ss.str());

    EXPECT_THROW(data.getValue("another_value"), std::invalid_argument);

}

