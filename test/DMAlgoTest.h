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



struct Driver : public testing::Test {

    // TODO: parameters
    double _a =  1.0;
    double _b = -2.0;

};

struct driver_state {

    double v;
    double vDes;
    double delta;
    double freeReaction;

    friend std::ostream& operator<< (std::ostream& os, const driver_state& st) {
        return os << "v=" << st.v << ", vDes=" << st.vDes << ", delta=" << st.delta
            << "\n\tfree=" << st.freeReaction;
    }

};

struct DMAlgoTest : public Driver, testing::WithParamInterface<driver_state> {

    DMAlgoTest() = default;

};


TEST_P(DMAlgoTest, IDMFree) {

    auto as = GetParam();
    double a = agmod::IDMSpeedReaction(as.v, as.vDes, as.delta, _a, _b);

    if(as.freeReaction == INFINITY)
        EXPECT_EQ(INFINITY, a);
    else
        EXPECT_NEAR(as.freeReaction, a, 1e-4);

}


INSTANTIATE_TEST_CASE_P(Default, DMAlgoTest,
    testing::Values(

            // velocity is zero
            driver_state{0.0, 10.0, 4.0, 1.0},
            driver_state{0.0, 100.0, 4.0, 1.0},

            // desired velocity is zero
            driver_state{10.0, 0.0, 4.0, INFINITY},
            driver_state{0.0, 0.0, 4.0, 0.0},

            // parameter is zero
            driver_state{10.0, 20.0, 0.0, 0.0},

            // desired and current velocity are equal
            driver_state{10.0, 10.0, 4.0, 0.0},
            driver_state{100.0, 100.0, 4.0, 0.0},

            // some other checks
            driver_state{10.0, 20.0, 4.0,  0.9375},
            driver_state{10.0, 30.0, 4.0,  0.9877},
            driver_state{20.0, 10.0, 4.0, -2.0},
            driver_state{20.0, 30.0, 4.0,  0.8025}
    )
);


#endif //SIMULATION_FRAMEWORK_MODELTEST_H

#pragma clang diagnostic pop