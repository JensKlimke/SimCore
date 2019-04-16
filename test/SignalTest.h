//
// Created by klimke on 16.04.2019.
//

#ifndef SIMCORE_SIGNALTEST_H
#define SIMCORE_SIGNALTEST_H

#include <gtest/gtest.h>
#include <models/value/SignalCurve.h>
#include <models/value/SignalTube.h>
#include <models/value/ValueExceed.h>
#include <models/value/ValueOutOfTube.h>
#include <core/IStopCondition.h>


TEST(SignalTestBasic, SignalCurve) {

    SignalCurve sc;

    EXPECT_FALSE(sc.isSet());
    sc.define({0.0, 10.0, 20.0, 30.0}, {1.0, 2.0, 2.0, -1.0});
    EXPECT_TRUE(sc.isSet());

    // check interpolate function
    EXPECT_NEAR( 1.0, sc.interpolate(0.0), EPS_DISTANCE);
    EXPECT_NEAR( 1.1, sc.interpolate(1.0), EPS_DISTANCE);
    EXPECT_NEAR( 1.5, sc.interpolate(5.0), EPS_DISTANCE);
    EXPECT_NEAR( 2.0, sc.interpolate(10.0), EPS_DISTANCE);
    EXPECT_NEAR( 2.0, sc.interpolate(11.0), EPS_DISTANCE);
    EXPECT_NEAR( 2.0, sc.interpolate(20.0), EPS_DISTANCE);
    EXPECT_NEAR( 1.7, sc.interpolate(21.0), EPS_DISTANCE);
    EXPECT_NEAR(-1.0, sc.interpolate(30.0), EPS_DISTANCE);

    EXPECT_NEAR( 1.0, sc.interpolate(-1e-10), EPS_DISTANCE);
    EXPECT_NEAR(-1.0, sc.interpolate(30.0 + 1e-10), EPS_DISTANCE);

    EXPECT_THROW(sc.interpolate(-1e-4), std::invalid_argument);
    EXPECT_THROW(sc.interpolate(30.0 + 1e-4), std::invalid_argument);


    // check previous function
    EXPECT_NEAR( 1.0, sc.previous(0.0), EPS_DISTANCE);
    EXPECT_NEAR( 1.0, sc.previous(1.0), EPS_DISTANCE);
    EXPECT_NEAR( 1.0, sc.previous(5.0), EPS_DISTANCE);
    EXPECT_NEAR( 2.0, sc.previous(10.0), EPS_DISTANCE);
    EXPECT_NEAR( 2.0, sc.previous(11.0), EPS_DISTANCE);
    EXPECT_NEAR( 2.0, sc.previous(20.0), EPS_DISTANCE);
    EXPECT_NEAR( 2.0, sc.previous(21.0), EPS_DISTANCE);
    EXPECT_NEAR( 2.0, sc.previous(29.0), EPS_DISTANCE);
    EXPECT_NEAR(-1.0, sc.previous(30.0), EPS_DISTANCE);
    EXPECT_NEAR(-1.0, sc.previous(35.0), EPS_DISTANCE);

    EXPECT_THROW(sc.previous(-1e-4), std::invalid_argument);


    // check next function
    EXPECT_NEAR( 1.0, sc.next(-1.0), EPS_DISTANCE);
    EXPECT_NEAR( 2.0, sc.next( 0.0), EPS_DISTANCE);
    EXPECT_NEAR( 2.0, sc.next( 1.0), EPS_DISTANCE);
    EXPECT_NEAR( 2.0, sc.next( 5.0), EPS_DISTANCE);
    EXPECT_NEAR( 2.0, sc.next(10.0), EPS_DISTANCE);
    EXPECT_NEAR( 2.0, sc.next(11.0), EPS_DISTANCE);
    EXPECT_NEAR(-1.0, sc.next(20.0), EPS_DISTANCE);
    EXPECT_NEAR(-1.0, sc.next(21.0), EPS_DISTANCE);
    EXPECT_NEAR(-1.0, sc.next(29.0), EPS_DISTANCE);
    EXPECT_NEAR(-1.0, sc.next(30.0), EPS_DISTANCE);

    EXPECT_THROW(sc.next(30 + 1e-4), std::invalid_argument);


    // check hit function
    EXPECT_TRUE(sc.hit(0.0,  1.0));
    EXPECT_TRUE(sc.hit(1.0,  1.1));
    EXPECT_TRUE(sc.hit(5.0,  1.5));
    EXPECT_TRUE(sc.hit(10.0,  2.0));
    EXPECT_TRUE(sc.hit(11.0,  2.0));
    EXPECT_TRUE(sc.hit(20.0,  2.0));
    EXPECT_TRUE(sc.hit(21.0,  1.7));
    EXPECT_TRUE(sc.hit(30.0, -1.0));

    EXPECT_FALSE(sc.hit(0.0,  0.0));
    EXPECT_FALSE(sc.hit(5.0,  2.5));
    EXPECT_FALSE(sc.hit(30.0, -1.5));
    EXPECT_FALSE(sc.hit(30.0, -1.5, 0.4));

    EXPECT_TRUE(sc.hit(0.0,  0.0, 1.1));
    EXPECT_TRUE(sc.hit(5.0,  2.5, 1.1));
    EXPECT_TRUE(sc.hit(30.0, -1.5, 0.6));

    EXPECT_TRUE(sc.hit(-1e-10,  1.0));
    EXPECT_TRUE(sc.hit(30.0 + 1e-10, -1.0));

}


TEST(SignalTestBasic, SignalTube) {

    SignalTube sc;

    EXPECT_FALSE(sc.isSet());
    sc.defineUpper({0.0, 10.0, 20.0, 30.0}, {2.0, 3.0, 3.0,  0.0});
    EXPECT_FALSE(sc.isSet());
    sc.defineLower({0.0, 10.0, 20.0, 30.0}, {0.0, 1.0, 1.0, -2.0});
    EXPECT_TRUE(sc.isSet());

    EXPECT_NEAR( 1.0, sc.center(0.0),  EPS_DISTANCE);
    EXPECT_NEAR( 1.1, sc.center(1.0),  EPS_DISTANCE);
    EXPECT_NEAR( 1.5, sc.center(5.0),  EPS_DISTANCE);
    EXPECT_NEAR( 2.0, sc.center(10.0), EPS_DISTANCE);
    EXPECT_NEAR( 2.0, sc.center(11.0), EPS_DISTANCE);
    EXPECT_NEAR( 2.0, sc.center(20.0), EPS_DISTANCE);
    EXPECT_NEAR( 1.7, sc.center(21.0), EPS_DISTANCE);
    EXPECT_NEAR(-1.0, sc.center(30.0), EPS_DISTANCE);

    EXPECT_NEAR( 1.0, sc.center(-1e-10),       EPS_DISTANCE);
    EXPECT_NEAR(-1.0, sc.center(30.0 + 1e-10), EPS_DISTANCE);

    EXPECT_THROW(sc.center(-1e-4),       std::invalid_argument);
    EXPECT_THROW(sc.center(30.0 + 1e-4), std::invalid_argument);


    // check function in()
    EXPECT_TRUE(sc.in( 0.0, 1.0));
    EXPECT_TRUE(sc.in( 1.0, 1.1));
    EXPECT_TRUE(sc.in( 5.0, 1.5));
    EXPECT_TRUE(sc.in(30.0, -1.0));
    EXPECT_TRUE(sc.in(-1e-10, 1.0));
    EXPECT_TRUE(sc.in(30.0 + 1e-10, -1.0));

    EXPECT_TRUE(sc.in( 0.0, 2.0));
    EXPECT_TRUE(sc.in( 1.0, 2.1));
    EXPECT_TRUE(sc.in( 5.0, 2.49999));
    EXPECT_FALSE(sc.in( 5.0, 2.50001));

    EXPECT_FALSE(sc.in( 5.0, 4.5));
    EXPECT_FALSE(sc.in(30.0, -4.0));
    EXPECT_FALSE(sc.in(-1e-10, 5.0));
    EXPECT_FALSE(sc.in(30.0 + 1e-10, 1.0));
}



class SignalTest : public ::testing::Test, public sim::IModel {

protected:

    // create objects
    BasicTimer timer;
    TimeIsUp stop;
    ::sim::Loop sim;

    double x     = 0.0;
    double value = 0.0;

    void SetUp() override {

        // set parameters
        timer.setTimeStepSize(1.0);
        stop.setStopTime(10.0);

        // set timer and stop condition
        sim.setTimer(&timer);
        sim.addStopCondition(&stop);

        // models
        sim.addModel(&stop);

    }


private:

    bool step(double simTime) override {
        x += 1.0;
        value = x * x;

        std::cout << x << " " << value << std::endl;

    }


public:

    void initialize(double initTime) override {
        value = 0.0;
        x = 0.0;
    }

    void terminate(double simTime) override {
    }

};


TEST_F(SignalTest, ValueExceed) {

    // check value to exceed
    ValueExceed<double> ex;
    ex.setValueAndLimit(&value, 25.1, ValueExceed<double>::Mode::OBJECTIVES_MISSED);

    // add to sim
    sim.addModel(&ex);
    sim.addStopCondition(&ex);

    // add value limiter
    sim.addModel(this);

    // run sim
    sim.run();
    EXPECT_EQ(ex.getCode(), ::sim::IStopCondition::StopCode::OBJECTIVES_MISSED);
    EXPECT_DOUBLE_EQ(6.0, timer.time());
    EXPECT_DOUBLE_EQ(49.0, value);


    // change mode
    ex.setValueAndLimit(&value, 25.1);

    // run sim again
    sim.run();
    EXPECT_EQ(ex.getCode(), ::sim::IStopCondition::StopCode::SIM_ENDED);
    EXPECT_EQ(6.0, timer.time());
    EXPECT_DOUBLE_EQ(49.0, value);


    // change limit
    ex.setValueAndLimit(&value, 100.1);

    // run sim again
    sim.run();
    EXPECT_EQ(ex.getCode(), ::sim::IStopCondition::StopCode::NONE);
    EXPECT_EQ(10.0, timer.time());
    EXPECT_DOUBLE_EQ(121.0, value);

}


TEST_F(SignalTest, OutOfTube) {

    // check value to exceed
    ValueOutOfTube tube;
    tube.defineLower({0.0, 10.0}, {-0.1, -0.1});
    tube.defineUpper({0.0, 10.0}, { 1.1,  1.1});

    // set values
    tube.setValues(&x, &value);

    // add to sim
    sim.addModel(&tube);
    sim.addStopCondition(&tube);

    // add value limiter
    sim.addModel(this);


    // run sim
    sim.run();
    EXPECT_EQ(tube.getCode(), ::sim::IStopCondition::StopCode::OBJECTIVES_MISSED);
    EXPECT_EQ(2.0, timer.time());
    EXPECT_DOUBLE_EQ(9.0, value);


    // change limits
    tube.defineLower({0.0, 2.5, 5.0, 7.5, 10.0}, {-0.1, 0.0,   6.25, 25.0,   56.25});
    tube.defineUpper({0.0, 2.5, 5.0, 7.5, 10.0}, { 0.1, 6.35, 25.1,  56.35, 100.1 });

    // run sim again
    sim.run();
    EXPECT_EQ(tube.getCode(), ::sim::IStopCondition::StopCode::NONE);
    EXPECT_EQ(10.0, timer.time());
    EXPECT_DOUBLE_EQ(121.0, value);

}


#endif //SIMCORE_SIGNALTEST_H
