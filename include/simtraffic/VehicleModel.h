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
// Created by Jens Klimke on 2020-03-06.
// Contributors:
//


#ifndef SIMCORE_VEHICLE_MODEL_H
#define SIMCORE_VEHICLE_MODEL_H

#include <algorithm>
#include <simbasic/Spline.h>
#include "UnitModel.h"

namespace simtraffic {

    class VehicleModel : public UnitModel {

    public:

        struct {
            simbasic::Spline high{};
            simbasic::Spline low{};
            simbasic::Spline brake{};
            simbasic::Spline steer{};
        } parameters;


        double steering = 0.0; // The actual steering (no unit, -1: full right, 0: straight, 1: full left)
        double pedal = 0.0; // The actual pedal value (no unit, -1..0: braking, 0..1 throttle)



        /**
         * Constructor
         */
        VehicleModel() = default;


        /**
         * Returns the acceleration resulting from the pedal value
         * @param v Velocity state
         * @param p Pedal value
         * @return The resulting acceleration
         */
        double longitudinalForwards(double v, double p) const {

            // get max and min acceleration
            auto aMin = parameters.low(v);
            auto aMax = parameters.high(v);
            auto aBr  = parameters.brake(v);

            // limit pedal value
            auto drive = std::min(1.0, std::max(0.0,  p));
            auto brake = std::min(1.0, std::max(0.0, -p));

            // calculate acceleration
            return drive * aMax + (1.0 - drive) * aMin + brake * aBr;

        }


        /**
         * Returns the pedal value needed to accelerate as desired
         * @param v Velocity state
         * @param a Desired acceleration
         * @return The required pedal value
         */
        double longitudinalBackwards(double v, double a) const {

            // calculate acceleration points
            auto aMin = parameters.low(v);
            auto aMax = parameters.high(v);
            auto aBr = parameters.brake(v);

            // calculate pedals
            if (aMin < a)
                return (a - aMin) / (aMax - aMin);
            else
                return (2.0 * aMin - a) / (aBr - aMin);

        }


        /**
         * Calculates the curvature based on the velocity state and the steering value
         * @param v Velocity state
         * @param s Steering value
         * @return The curvature resulting from steering and velocity
         */
        double lateralForwards(double v, double s) const {

            auto c = parameters.steer(v);
            return std::min(1.0, std::max(-1.0, s)) * c;

        }


        /**
         * Calculates the steering value based on the velocity state and the curvature value
         * @param v Velocity state
         * @param c Curvature value
         * @return The steering value resulting from curvature and velocity
         */
        double lateralBackwards(double v, double c) const {

            return c / parameters.steer(v);

        }



        /**
         * Calculates a forward step of the vehicle model
         * @param time Actual time
         * @param deltaTime Time step size
         */
        void step(double time, double deltaTime) override {

            // calculate acceleration
            acceleration = longitudinalForwards(velocity, pedal);
            curvature = lateralForwards(velocity, steering);

            // integration
            UnitModel::step(time, deltaTime);

        }


    };

}


#endif // SIMCORE_VEHICLE_MODEL_H
