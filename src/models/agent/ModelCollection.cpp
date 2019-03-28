//
// Created by Jens Klimke on 2019-03-24.
//

#include "ModelCollection.h"

namespace agmod {


    double IDMspeedReaction(double v, double v0, double delta) {

        return 1.0 - pow(v / v0, delta);

    }


    double distanceDownscale(double ds, double dsMax, double delta) {

        return pow(dsMax / ds, delta);

    }

}