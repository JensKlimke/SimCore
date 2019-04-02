//
// Created by Jens Klimke on 2019-03-24.
//

#include "ModelCollection.h"

#ifndef SIGN
#define SIGN(X) X == 0.0 ? 0.0 : (X < 0.0 ? -1.0 : 1.0)
#endif

namespace agmod {


    double IDMspeedReaction(double v, double v0, double delta) {

        auto dv = v0 - v;
        return SIGN(dv) * (1.0 - pow(1.0 - fabs(dv) / v0, delta));

    }


    double distanceDownscale(double ds, double dsMax, double delta) {

        return pow(dsMax / ds, delta);

    }

}