// Copyright (c) 2021 Jens Klimke.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// Created by Jens Klimke on 2020-03-11.
// Contributors:
//


#ifndef SIMCORE_SETUP_VEHICLE_MODEL_H
#define SIMCORE_SETUP_VEHICLE_MODEL_H

#include "VehicleModel.h"

namespace simcore {
    namespace traffic {

        /** @brief Sets the parameters of the given vehicle model with the given configuration.
         * The setup is done by two spline (high and low) which represents the acceleration functions at the actual velocity
         * for no pedal input (a(v,p=0) = a_low(v)) and full pedal value (a(v,p=1) = a_high(v)), respectively. The curves are steady down to 2nd
         * derivative order and have the following conditions:
         * - a_high(v=0) = aMax, a_high'(v=0) = 0
         * - a_high(v=vMax) = 0
         * - a_high(v>=v0) = p/v - a_low(v)
         * - a_low(v=0) = aIdle
         * - a_low(v=vIdle) = 0
         * - C2 continuity for a_high(v=v0) and a_low(v=vIdle)
         * @param vehicle Pointer to the vehicle model to be setup.
         * @param aIdle The acceleration of the vehicle when v=0 and pedal in idle state (p=0).
         * @param vIdle The velocity the vehicle reaches when p=0.
         * @param v0 The minimum velocity from which the full relative power is used.
         * @param p The power the vehicle can bring to the ground per mass unit (relative power).
         * @param vMax The maximum velocity the vehicle can reach (with p=1).
         * @param aMax The maximum acceleration (at v=0, p=1). The actual maximum acceleration might be higher for 0 < v < v0.
         * @param aMin The maximum deceleration (aMin < 0). The actual minimum acceleration might be lower due to the resistance forces.
         */
        void inline
        setupVehicleByVelocityAndAccelerationStates(VehicleModel *vehicle,
                                                    double aIdle,
                                                    double vIdle,
                                                    double v0,
                                                    double p,
                                                    double vMax,
                                                    double aMax,
                                                    double aMin) {

            using namespace simbasic;

            auto d0 = -p * vIdle * vIdle / (vIdle * vIdle * vMax - vMax * vMax * vMax);
            auto d2 = p / (vIdle * vIdle * vMax - vMax * vMax * vMax);

            // low, first section
            auto a0 = aIdle;
            auto a1 = (-3 * aIdle - 3 * d2 * (vIdle * vIdle)) / vIdle;
            auto a2 = (3 * aIdle + 4 * d2 * (vIdle * vIdle)) / (vIdle * vIdle);
            auto a3 = (-aIdle - d2 * (vIdle * vIdle)) / (vIdle * vIdle * vIdle);

            // define polynomial
            Polynomial pLow0({a0, a1, a2, a3});
            Polynomial pLow1({d0, 0.0, d2});

            // calculate
            auto aL = pLow1(v0) + p / v0;
            auto aLd = pLow1.derivative(v0) - p / (v0 * v0);
            auto aLdd = pLow1.derivative(v0, 2) + 2.0 * p / (v0 * v0 * v0);

            // high, first section
            auto b0 = aMax;
            auto b1 = 0.0;
            auto b2 = (12 * aL - 6 * aLd * v0 + aLdd * (v0 * v0) - 12 * aMax) / (2 * (v0 * v0));
            auto b3 = (-8 * aL + 5 * aLd * v0 - aLdd * (v0 * v0) + 8 * aMax) / (v0 * v0 * v0);
            auto b4 = (6 * aL - 4 * aLd * v0 + aLdd * (v0 * v0) - 6 * aMax) / (2 * (v0 * v0 * v0 * v0));

            // polynomial for 0..v0 transition
            Polynomial pHigh0({b0, b1, b2, b3, b4});
            Polynomial pHigh1(pLow1); pHigh1.add(-1, p); // p/v - d0 - d2 * v^2

            // define 0..v0
            vehicle->parameters.low = simbasic::Spline(std::move(pLow0));
            vehicle->parameters.low.add(vIdle, std::move(pLow1));

            // define 0..v0
            vehicle->parameters.high = Spline(std::move(pHigh0));
            vehicle->parameters.high.add(v0, std::move(pHigh1));

            // set maximum deceleration
            vehicle->parameters.brake.emplace_back(simbasic::SplineSegment{INFINITY, Polynomial(aMin)});

        }



        /**
         * Sets the parameters of the given vehicle model with the given configuration.
         * The setup is done by two spline which represents the curvature functions at the actual velocity
         * @param vehicle Pointer to the vehicle model to be setup
         * @param kappa0 Curvature at v=0
         * @param kappaMax Curvature at maximum velocity
         * @param vMax Maximum velocity
         */
        void inline setupVehicleByCurvature(VehicleModel *vehicle, double kappa0, double kappaMax, double vMax) {

            // create polynomial
            simbasic::Polynomial poly;
            poly.add(0, kappa0);
            poly.add(2, (kappaMax - kappa0) / (vMax * vMax));

            // set spline
            vehicle->parameters.steer.emplace_back(simbasic::SplineSegment{INFINITY, poly});

        }

    }

}

#endif // SIMCORE_SETUP_VEHICLE_MODEL_H
