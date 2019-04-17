//
// Created by klimke on 16.04.2019.
//

#ifndef SIMCORE_REPORTERTEST_H
#define SIMCORE_REPORTERTEST_H

#include <gtest/gtest.h>
#include <core/IComponent.h>
#include <core/Loop.h>
#include <core/IStopCondition.h>
#include <models/value/SignalCurve.h>
#include <models/value/SignalTube.h>
#include <models/value/ValueExceed.h>
#include <models/value/ValueOutOfTube.h>
#include <models/timers/BasicTimer.h>
#include <models/timers/TimeIsUp.h>
#include <models/reporters/TimeReporter.h>
#include <models/reporters/JsonReporter.h>

class ReporterTest : public ::testing::Test, public sim::IComponent {

protected:

    // create objects
    BasicTimer timer;
    TimeIsUp stop;
    ::sim::Loop loop;

    double x     = 0.0;
    double value = 0.0;

    void SetUp() override {

        // set parameters
        timer.setTimeStepSize(0.1);
        stop.setStopTime(10.0);

        // set timer and stop condition
        loop.setTimer(&timer);
        loop.addStopCondition(&stop);

        // models
        loop.addModel(&stop);

    }


private:

    bool step(double simTime) override {

        x += 1.0;
        value = x * x;

        return true;

    }


public:

    void initialize(double initTime) override {
        value = 0.0;
        x = 0.0;
    }

    void terminate(double simTime) override {
    }

};


TEST_F(ReporterTest, ReportTime) {

    // create out-stream
    std::stringstream ostr;

    // create synchronized model (in this case time reporter)
    TimeReporter timeRep;
    loop.addModel(&timeRep);

    // setup for time reporter
    timeRep.setTimeStepSize(5.0, 1.0);
    timeRep.ostream(ostr);

    // add another time reporter (for output in console)
    TimeReporter timeRepConsole;
    loop.addModel(&timeRepConsole);

    // setup for time reporter
    timeRepConsole.setTimeStepSize(1.0);

    // check status
    EXPECT_EQ(sim::Loop::Status::STOPPED, loop.getStatus());

    // initialize simulation
    loop.run();

    // check time and status
    EXPECT_NEAR(10.0, timer.time(), 1e-8);
    EXPECT_EQ(sim::Loop::Status::STOPPED, loop.getStatus());

    // check output of timer
    EXPECT_EQ("1s\n6s\n", ostr.str());

}



#endif //SIMCORE_REPORTERTEST_H
