//
// Created by Jens Klimke on 2019-03-24.
//

#include "ModelCollection.h"
#include <cmath>
#include <algorithm>

#ifndef DM_VELOCITY_LIMIT_STOP
#define DM_VELOCITY_LIMIT_STOP 10.0
#endif

#ifndef SIGN
#define SIGN(X) X == 0.0 ? 0.0 : (X < 0.0 ? -1.0 : 1.0)
#endif

namespace agmod {


    double IDMSpeedReaction(double v, double v0, double delta) {

        auto dv = v0 - v;
        return SIGN(dv) * (1.0 - pow(1.0 - fabs(dv) / v0, delta));

    }


    double distanceDownscale(double ds, double dsMax, double delta) {

        return pow(dsMax / ds, delta);

    }


    double IDMFollowReaction(double &aRes, double ds, double vTar, double v, double dsDes, double dsStop, double a, double b) {

        // calculate relative velocity
        auto dv = v - vTar;

        // calculate dynamic factors
        auto kDyn = v * dv / (2.0 * ds * sqrt(-b * a));
        auto kDes = dsDes / ds;
        auto kSt  = dsStop / ds;

        // scale down stopping target reaction and limit dynamic factor
        auto wSt  = fade(1.0 - (std::min(vTar, DM_VELOCITY_LIMIT_STOP) / DM_VELOCITY_LIMIT_STOP));
        auto wDes = fade(kDes);

        // calculate reaction
        aRes = std::max(0.0, wSt * kSt + kDes + kDyn);
        aRes *= aRes;


        // return scale down factor
        return std::max((1.0 - wDes), 0.0);

    }


    double fade(double x) {

        x = std::max(0.0, std::min(1.0, x));
        return 0.5 * (cos(x * M_PI - M_PI) + 1.0);

    }

}