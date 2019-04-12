//
// Created by klimke on 12.04.2019.
//

#ifndef SIMCORE_ALGOTEST_H
#define SIMCORE_ALGOTEST_H

#include <gtest/gtest.h>
#include <models/value/SignalCurve.h>

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


#endif //SIMCORE_ALGOTEST_H
