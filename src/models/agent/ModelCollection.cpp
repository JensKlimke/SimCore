//
// Created by Jens Klimke on 2019-03-24.
//

#include "ModelCollection.h"
#include <cmath>
#include <algorithm>
#include <iostream>


#ifndef MAX_DISTANCE
#define MAX_DISTANCE 1e9
#endif


#ifndef DM_VELOCITY_LIMIT_STOP
#define DM_VELOCITY_LIMIT_STOP 10.0
#endif

#ifndef SIGN
#define SIGN(X) X == 0.0 ? 0.0 : (X < 0.0 ? -1.0 : 1.0)
#endif

#ifndef MAX
#define MAX(X, Y) X >= Y ? X : Y
#endif

#ifndef MIN
#define MIN(X, Y) X <= Y ? X : Y
#endif

namespace agmod {


    double IDMSpeedPredictionReaction(double &aRes, double ds, double vDes, double v, double thwMax, double deltaPred,
                                      double delta, double a, double b) {

        // initialize values
        double ratio = 1.0;
        aRes  = 0.0;

        // only calculate prediction, when relevant
        if (ds >= 0.0) {

            // calculate ratio
            ratio = ds / (thwMax * vDes);
            ratio = std::pow(std::min(1.0, std::max(0.0, ratio)), deltaPred);

            // calculate reaction
            aRes = ::agmod::IDMSpeedReaction(v, vDes, delta, a, b);

        }

        return ratio;

    }


    double IDMSpeedReaction(double v, double v0, double delta, double a, double b) {

        auto dv = v0 - v;
        double aRes = SIGN(dv) * (1.0 - pow(1.0 - fabs(dv) / v0, delta));

        return aRes * (aRes < 0.0 ? -b : a);

    }


    double IDMFollowReaction(double &aRes, double ds, double vTar, double v, double T, double dsStop, double a, double b) {

        // initialize aRes
        aRes = 0.0;

        // abort if distance is too big
        if(ds > MAX_DISTANCE)
            return 1.0;

        // calculate relative velocity
        auto dv = v - vTar;

        // calculate dynamic factors
        auto kDyn = v * dv / (2.0 * ds * sqrt(-b * a));
        auto kDes = v * T / ds;
        auto kSt  = dsStop / ds;

        // scale down stopping target reaction and limit dynamic factor
        auto wSt  = fade(1.0 - (MIN(vTar, DM_VELOCITY_LIMIT_STOP) / DM_VELOCITY_LIMIT_STOP));
        auto wDes = fade(kDes);

        // calculate reaction
        aRes = MAX(0.0, wSt * kSt + kDes + kDyn);
        aRes *= -aRes;

        // return scale down factor
        return MAX((1.0 - wDes), 0.0);

    }


    double fade(double x) {

        x = MAX(0.0, MIN(1.0, x));
        return 0.5 * (cos(x * M_PI - M_PI) + 1.0);

    }

}