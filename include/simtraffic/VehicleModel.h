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
#include <simcore/IComponent.h>
#include "Unit.h"

namespace simtraffic {

    class VehicleModel : public Unit, public simcore::IComponent {

    public:

        struct {
            simbasic::Spline high{};
            simbasic::Spline low{};
            simbasic::Spline brake{};
            simbasic::Spline steer{};
        } parameters;


        /**
         * Constructor
         */
        VehicleModel() = default;


        /**
         * Returns the acceleration resulting from the pedal value
         * @param velocity Velocity state
         * @param pedal Pedal value
         * @return The resulting acceleration
         */
        double longitudinalForwards(double velocity, double pedal) const {

            // get max and min acceleration
            auto aMin = parameters.low(velocity);
            auto aMax = parameters.high(velocity);
            auto aBr  = parameters.brake(velocity);

            // limit pedal value
            auto drive = std::min(1.0, std::max(0.0, pedal));
            auto brake = std::min(1.0, std::max(0.0, -pedal));

            // calculate acceleration
            return drive * aMax + (1.0 - drive) * aMin + brake * aBr;

        }


        /**
         * Returns the pedal value needed to accelerate as desired
         * @param velocity Velocity state
         * @param acceleration Desired acceleration
         * @return The required pedal value
         */
        double longitudinalBackwards(double velocity, double acceleration) const {

            // calculate acceleration points
            auto aMin = parameters.low(velocity);
            auto aMax = parameters.high(velocity);
            auto aBr = parameters.brake(velocity);

            // calculate pedals
            if (aMin < acceleration)
                return (acceleration - aMin) / (aMax - aMin);
            else
                return (2.0 * aMin - acceleration) / (aBr - aMin);

        }


        /**
         * Calculates the curvature based on the velocity state and the steering value
         * @param velocity Velocity state
         * @param steering Steering value
         * @return The curvature resulting from steering and velocity
         */
        double lateralForwards(double velocity, double steering) const {

            auto c = parameters.steer(velocity);
            return std::min(1.0, std::max(-1.0, steering)) * c;

        }


        /**
         * Calculates the steering value based on the velocity state and the curvature value
         * @param velocity Velocity state
         * @param curvature Curvature value
         * @return The steering value resulting from curvature and velocity
         */
        double lateralBackwards(double velocity, double curvature) const {

            return curvature / parameters.steer(velocity);

        }


        /**
         * Initializes the vehicle
         * @param initTime Init time
         */
        void initialize(double initTime) override {

            // reset distance
            distance = 0.0;

            // reset distance counter
            getDistance();

        }


        /**
         * Calculates a forward step of the vehicle model
         * @param time Actual time
         * @param dt Time step size
         */
        void step(double time, double dt) override {

            // calculate acceleration
            acceleration = longitudinalForwards(velocity, pedal);
            velocity += acceleration * dt;

            // limit velocity
            if(velocity <= 0.0)
                velocity = 0.0;

            // limit acceleration
            if(velocity <= 0.0 && acceleration <= 0.0)
                acceleration = 0.0;

            // calculate distance
            auto ds = velocity * dt;
            distance += ds;

            // set yaw rate
            curvature = lateralForwards(velocity, steering);
            yawRate = curvature * velocity;

            // calculate change in position
            auto dSin = sin(yawRate * dt);
            auto dCos = cos(yawRate * dt);

            // calculate heading
            heading = {
                    heading.x * dCos - heading.y * dSin,
                    heading.y * dCos + heading.x * dSin,
                    0.0
            };

            // normalize (to avoid cumulated accuracy errors)
            auto l = 1.0 / sqrt(heading.x * heading.x + heading.y * heading.y);
            heading.x *= l;
            heading.y *= l;

            // position
            position.x += heading.x * ds;
            position.y += heading.y * ds;
            position.z = 0.0;

        }


    };

}


#endif // SIMCORE_VEHICLE_MODEL_H
