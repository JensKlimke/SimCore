//
// Created by Jens Klimke on 2019-03-24.
//

#ifndef SIMULATION_FRAMEWORK_MODELCOLLECTION_H
#define SIMULATION_FRAMEWORK_MODELCOLLECTION_H

#include <math.h>

namespace agmod {


    /**
     * The reaction based on the current speed and the desired speed. The model is defined in
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
    double IDMSpeedReaction(double v, double v0, double delta);


    /**
     * Calculates the down-scale factor dependent on the distance
     * @param ds Distance
     * @param dsMax Maximum distance
     * @param delta Parameter
     * @return down-scale factor
     */
    double distanceDownscale(double ds, double dsMax, double delta);



    /**
     * Calculates the reaction on vehicles during approaching and following
     * @param ds Distance to the target vehicle (positive when following, in *m*)
     * @param vTar Velocity of the target vehicle (NOT relative velocity, in *m/s*)
     * @param v Velocity of the ego vehicle (in *m/s*)
     * @param dsDes Desired following distance (in *m*)
     * @param dsStop Desired distance when stopping (in *m*)
     * @param a Desired comfortable maximum acceleration (in *m/s^2*)
     * @param b Desired comfortable maximum deceleration (negative, in *m/s^2*)
     * @return The resultant acceleration and the scale down factor for cruising
     */
    double IDMFollowReaction(double &aRes, double ds, double vTar, double v, double dsDes, double dsStop, double a, double b);


    /**
     * Fades a value between one and zero with cos
     * @param x Input value
     * @return Output value
     */
    double fade(double x);


}

#endif //SIMULATION_FRAMEWORK_MODELCOLLECTION_H
