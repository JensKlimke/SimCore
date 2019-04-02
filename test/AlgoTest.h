//
// Created by Jens Klimke on 2019-03-24.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cppcoreguidelines-avoid-goto"

#ifndef SIMULATION_FRAMEWORK_MODELTEST_H
#define SIMULATION_FRAMEWORK_MODELTEST_H

#include <gtest/gtest.h>
#include <models/agent/ModelCollection.h>
#include <cmath>
#include "models/value/SignalCurve.h"

TEST(AlgoTest, IDMFree) {

    // velocity is zero
    EXPECT_DOUBLE_EQ(1.0, agmod::IDMspeedReaction(0.0, 10.0, 4.0));
    EXPECT_DOUBLE_EQ(1.0, agmod::IDMspeedReaction(0.0, 100.0, 4.0));

    // desired velocity is zero
    EXPECT_DOUBLE_EQ(INFINITY, agmod::IDMspeedReaction(10.0, 0.0, 4.0));
    EXPECT_DOUBLE_EQ(0.0, agmod::IDMspeedReaction(0.0, 0.0, 4.0));

    // parameter is zero
    EXPECT_DOUBLE_EQ(0.0, agmod::IDMspeedReaction(10.0, 20.0, 0.0));

    // desired and current velocity are equal
    EXPECT_DOUBLE_EQ(0.0, agmod::IDMspeedReaction(10.0, 10.0, 4.0));
    EXPECT_DOUBLE_EQ(0.0, agmod::IDMspeedReaction(100.0, 100.0, 4.0));

    // some checks
    EXPECT_NEAR( 0.9375, agmod::IDMspeedReaction(10.0, 20.0, 4.0), 1e-4);
    EXPECT_NEAR( 0.9877, agmod::IDMspeedReaction(10.0, 30.0, 4.0), 1e-4);
    EXPECT_NEAR(-1.0,    agmod::IDMspeedReaction(20.0, 10.0, 4.0), 1e-4);
    EXPECT_NEAR( 0.8025, agmod::IDMspeedReaction(20.0, 30.0, 4.0), 1e-4);

}


TEST(AlgoTest, SignalCurve) {

    SignalCurve sc;
    sc.define({0.0, 10.0, 20.0, 30.0}, {1.0, 2.0, 2.0, -1.0});
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

}

#endif //SIMULATION_FRAMEWORK_MODELTEST_H

#pragma clang diagnostic pop