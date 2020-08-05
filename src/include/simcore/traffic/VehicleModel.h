// Copyright (c) 2020 Jens Klimke (jens.klimke@rwth-aachen.de). All rights reserved.
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
// Created by Jens Klimke on 2020-08-04.
//


#ifndef SIMCORE_VEHICLE_MODEL_H
#define SIMCORE_VEHICLE_MODEL_H

#include <cmath>
#include "Unit.h"

namespace sim::traffic {

    class VehicleModel : public Unit {

    public:

        enum class IndicatorState {OFF, LEFT, RIGHT, HAZARD};       //!< Enumeration to store the indicator state
        enum class ShifterPosition {PARK, NEUTRAL, DRIVE, REVERSE}; //!< Enumeration to store the shifter position

        /** A struct to store the continuous inputs to the vehicle (primary inputs) */
        struct ContinuousInput {
            double drive = 0.0;    //!< Drive power request by the driver or driving system [0..1] (0: zero drive power, 1: full drive power, unitless)
            double brake = 0.0;    //!< Brake torque request by the driver or driving system [0..1] (0: zero brake torque, 1: full brake torque, unitless)
            double steering = 0.0; //!< Steering request by the driver or driving system [-1..1] (0: straight, -1/1: full steering to left/right, unitless)
        };

        /** A struct to store the vehicle parameters */
        struct Parameters {
            double wheelBase = 3.0;             //!< Wheel base (distance of axles, in *m*)
            double maxSteerAngle = M_PI_4;      //!< Maximum absolute wheel steer angle (in *rad*)
            double maxDrivePower = 4.13e5;      //!< Maximum drive power (in *W*)
            double maxDriveTorque = 775.0;      //!< Maximum drive torque (in *Nm*)
            double maxBrakeTorque = 1000.0;     //!< Maximum brake torque (in *Nm*)
            double maxAcceleration = 9.81;      //!< Maximum absolute effective acceleration (in *m/s^2*)
            double maxVelocity = 69.444;        //!< Maximum forward velocity (default: 250 kph, in *m/s*)
            double maxReverseVelocity = 10.0;   //!< Maximum reverse velocity (in *m/s*)
            double selfSteeringGradient = 0.08; //!< Self-steering gradient of the vehicle
        };

        /** A struct to store the vehicle state */
        struct State {
            IndicatorState indicatorState = IndicatorState::OFF;     //!< The actual indicator state
            ShifterPosition shifterPosition = ShifterPosition::PARK; //!< The actual shifter position
        };


        void step(double dt) {

            // get state
            auto v = Unit::state.velocity;
            auto dPsi = Unit::state.yawRate;
            auto psi = Unit::state.yawAngle;
            auto pos = Unit::state.position;
            auto a = Unit::state.acceleration;

            // calculate longitudinal motion
            auto ds = 0.5 * a * dt * dt + v * dt;

            // calculate 2D motion
            auto dx = sin(psi) * v + cos(psi) * dPsi * ds;
            auto dy = cos(psi) * v - sin(psi) * dPsi * ds;

            // update position and yaw angle
            pos.x += dx * dt;
            pos.y += dy * dt;
            psi += dPsi * dt;

            // steering model (linear bicycle model)
            auto delta = range(input.steering, -1.0, 1.0) * parameters.maxSteerAngle;
            dPsi = v * delta / parameters.wheelBase;

            // drive train model
            v += a * dt;
            auto force = 0.0; // TODO

            // update state
            Unit::state.acceleration = a;
            Unit::state.velocity = v;
            Unit::state.yawRate = dPsi;
            Unit::state.yawAngle = psi;
            Unit::state.position = pos;

        }


    protected:

        Parameters parameters;
        ContinuousInput input;
        State state;


        /**
         * Limits the given value to a minumum and maximum value
         * @param x Value to be limited
         * @param xMin Minimum value
         * @param xMax Maximum value
         * @return The limited value
         */
        double range(double x, double xMin, double xMax) {

            return std::min(std::max(x, xMin), xMax);

        }

    };

}

#endif //SIMCORE_VEHICLE_MODEL_H
