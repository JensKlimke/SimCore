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
                double maximumDeceleration = -10.0;
            } parameters;

            struct {
                double pedal = 0.0;
            } input;


            VehicleModel() = default;


            void reset(double time) {

                // set time
                _time = time;

                // reset distance
                state.distance = 0.0;

            }


            void setup(double aIdle, double v0, double v1, double p, double vMax, double aMax) {

                using namespace simbasic;

                // define polynomials
                Polynomial pLow0({aIdle,
                                  -3 * aIdle / v0,
                                   3 * aIdle / (v0 * v0) - p / (vMax * vMax * vMax),
                                  -aIdle / (v0 * v0 * v0)});

                Polynomial pLow1;
                pLow1.emplace_back(PolyParam{2, -p / (vMax * vMax * vMax)});

                Polynomial pHigh0({aMax,
                                    0.0,
                                   -6 * aMax / (v1 * v1) - p / (vMax * vMax * vMax) + 10 * p / (v1 * v1 * v1),
                                   (8 * aMax * v1 - 15 * p) / (v1 * v1 * v1 * v1),
                                   3 * (-aMax * v1 + 2 * p) / (v1 * v1 * v1 * v1 * v1)});

                Polynomial pHigh1;
                pHigh1.emplace_back(PolyParam{-1, p});
                pHigh1.emplace_back(PolyParam{2, -p / (vMax * vMax * vMax)});


                // define 0..v0
                parameters.low.emplace_back(simbasic::SplineSegment{v0, std::move(pLow0)});
                parameters.low.emplace_back(simbasic::SplineSegment{INFINITY, pLow1});

                // define 0..v0
                parameters.high.emplace_back(simbasic::SplineSegment{v1, std::move(pHigh0)});
                parameters.high.emplace_back(simbasic::SplineSegment{INFINITY, pHigh1});

            }


            /**
             * Returns the acceleration resulting from the pedal value
             * @param velocity Velocity state
             * @param pedal Pedal value
             * @return The resulting acceleration
             */
            double forwards(double velocity, double pedal) const {

                // get max and min acceleration
                auto aMin = parameters.low(velocity);
                auto aMax = parameters.high(velocity);

                // limit pedal value
                auto drive = std::min(1.0, std::max(0.0, pedal));
                auto brake = std::min(1.0, std::max(0.0, -pedal));

                // calculate acceleration
                return drive * aMax + (1.0 - drive) * aMin + brake * parameters.maximumDeceleration;

            }


            /**
             * Returns the pedal value needed to accelerate as desired
             * @param velocity Velocity state
             * @param acceleration Desired acceleration
             * @return The required pedal value
             */
            double backwards(double velocity, double acceleration) const {

                // calculate acceleration points
                auto aMin = parameters.low(velocity);
                auto aMax = parameters.high(velocity);
                auto aBr = parameters.maximumDeceleration;

                // calculate pedals
                if (aMin < acceleration)
                    return (acceleration - aMin) / (aMax - aMin);
                else
                    return (2.0 * aMin - acceleration) / (aBr - aMin);

            }


            /**
             * Calculates a forward step of the vehicle model
             * @param time Actual time
             */
            void step(double time) {

                // calculate acceleration
                state.acceleration = forwards(state.velocity, input.pedal);
                state.velocity += state.acceleration * (time - _time);

                // limit velocity
                state.velocity = state.velocity < 0.0 ? 0.0 : state.velocity;

                // set time
                _time = time;

            }


        };

    }
}


#endif // SIMCORE_VEHICLE_MODEL_H
