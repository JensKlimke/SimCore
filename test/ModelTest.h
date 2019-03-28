//
// Created by Jens Klimke on 2019-03-24.
//

#ifndef SIMULATION_FRAMEWORK_MODELTEST_H
#define SIMULATION_FRAMEWORK_MODELTEST_H

#include <gtest/gtest.h>
#include <models/agent/ModelCollection.h>
#include <cmath>

TEST(ModelTest, IDMFree) {

    // velocity is zero
    EXPECT_DOUBLE_EQ(1.0, agmod::IDMspeedReaction(0.0, 10.0, 4.0));
    EXPECT_DOUBLE_EQ(1.0, agmod::IDMspeedReaction(0.0, 100.0, 4.0));

    // desired velocity is zero
    EXPECT_DOUBLE_EQ(-INFINITY, agmod::IDMspeedReaction(10.0, 0.0, 4.0));
    EXPECT_TRUE(std::isnan(agmod::IDMspeedReaction(0.0, 0.0, 4.0)));

    // parameter is zero
    EXPECT_DOUBLE_EQ(0.0, agmod::IDMspeedReaction(10.0, 20.0, 0.0));

    // desired and current velocity are equal
    EXPECT_DOUBLE_EQ(0.0, agmod::IDMspeedReaction(10.0, 10.0, 4.0));
    EXPECT_DOUBLE_EQ(0.0, agmod::IDMspeedReaction(100.0, 100.0, 4.0));

    // some checks
    EXPECT_NEAR(0.9375, agmod::IDMspeedReaction(10.0, 20.0, 4.0), 1e-4);
    EXPECT_NEAR(0.9877, agmod::IDMspeedReaction(10.0, 30.0, 4.0), 1e-4);
    EXPECT_NEAR( -15.0, agmod::IDMspeedReaction(20.0, 10.0, 4.0), 1e-4);
    EXPECT_NEAR(0.8025, agmod::IDMspeedReaction(20.0, 30.0, 4.0), 1e-4);

}

#endif //SIMULATION_FRAMEWORK_MODELTEST_H
