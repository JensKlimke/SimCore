//
// Created by Jens Klimke on 2019-03-24.
//

#ifndef SIMULATION_FRAMEWORK_MODELCOLLECTION_H
#define SIMULATION_FRAMEWORK_MODELCOLLECTION_H

#include <math.h>

namespace agmod {


    /**
     * The reaction (acceleration in *m/s^2*) based on the current speed and the desired speed. The model is defined in
     * the free part of the IDM model.
     * TODO: ref to IDM
     *
     * ${\dot {v}}_{\alpha }^{\text{free}}=a\,\left(1-\left({\frac {v_{\alpha }}{v_{0}}}\right)^{\delta }\right)$
     *
     * @param v  Current velocity (in *m/s*)
     * @param v0 Desired velocity (in *m/s*)
     * @param delta The parameter \delta
     * @return acceleration value
     */
    double IDMspeedReaction(double v, double v0, double delta);


    /**
     * Calculates the down-scale factor dependent on the distance
     * @param ds Distance
     * @param dsMax Maximum distance
     * @param delta Parameter
     * @return down-scale factor
     */
    double distanceDownscale(double ds, double dsMax, double delta);

}

#endif //SIMULATION_FRAMEWORK_MODELCOLLECTION_H
