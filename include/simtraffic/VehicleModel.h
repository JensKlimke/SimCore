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
#include "Unit.h"

namespace simcore {
    namespace traffic {

        class VehicleModel : public Unit {

        protected:

            double _time{};

        public:

            struct {
                simbasic::Spline high{};
                simbasic::Spline low{};
                simbasic::Spline brake{};
                simbasic::Spline steer{};
            } parameters;

            struct {
                double pedal = 0.0;
                double steering = 0.0;
            } input;


            /**
             * Constructor
             */
            VehicleModel() = default;


            /**
             * Resets the vehicle model
             * @param time Reset time
             */
            void reset(double time) {

                // set time
                _time = time;

                // reset distance
                distance = 0.0;

            }


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
             * Calculates a forward step of the vehicle model
             * @param time Actual time
             */
            void step(double time) {

                // time step
                auto dt = time - _time;

                // calculate acceleration
                acceleration = longitudinalForwards(velocity, input.pedal);
                velocity += acceleration * dt;

                // limit velocity
                velocity = velocity < 0.0 ? 0.0 : velocity;

                // calculate distance
                auto ds = velocity * dt;
                distance += ds;

                // set yaw rate
                curvature = lateralForwards(velocity, input.steering);
                yawRate = curvature * velocity;
                yawAngle += yawRate * dt;

                // position
                position.x += cos(yawAngle) * ds;
                position.y += sin(yawAngle) * ds;
                position.z = 0.0;

                // set time
                _time = time;

            }


        };

    }
}


#endif // SIMCORE_VEHICLE_MODEL_H
