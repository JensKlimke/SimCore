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

#ifndef EPS_SIM_TIME
#define EPS_SIM_TIME 1e-9
#endif

#ifndef EPS_VELOCITY
#define EPS_VELOCITY 1e-6
#endif

#ifndef ZERO_VELOCITY
#define ZERO_VELOCITY 1e-3
#endif

#ifndef range
#define range(X, MIN, MAX) (X < MIN ? MIN : (X < MAX ? X : MAX))
#endif


namespace sim::traffic {

    class VehicleModel {

    public:

        enum class IndicatorState {OFF, LEFT, RIGHT, HAZARD};       //!< Enumeration to store the indicator state
        enum class ShifterPosition {PARK, NEUTRAL, DRIVE, REVERSE}; //!< Enumeration to store the shifter position

        /** A struct to store the continuous inputs to the vehicle (primary inputs) */
        struct ContinuousInput {
            double drive = 0.0;    //!< Drive power request by the driver or driving system [0..1] (0: zero drive power, 1: full drive power, no unit)
            double brake = 0.0;    //!< Brake torque request by the driver or driving system [0..1] (0: zero brake torque, 1: full brake torque, no unit)
            double steering = 0.0; //!< Steering request by the driver or driving system [-1..1] (0: straight, -1/1: full steering to left/right, no unit)
        };

        /** A struct to store the vehicle parameters */
        struct Parameters {
            double maxCurvature;            //!< Maximum possible curvature with full steering (max. steering angle / wheel base, in *1/m*)
            double maxRelDrivePower;        //!< Maximum relative drive power (max. power / mass, in *W/kg*)
            double maxRelReverseDrivePower; //!< Maximum relative drive power when reversing (max. power / mass, in *W/kg*)
            double maxRelDriveTorque;       //!< Maximum relative drive torque (max. torque * wheel radius / mass, in *Nm/kg* = *m/s^2*)
            double maxRelBrakeTorque;       //!< Maximum relative brake torque (max. torque * wheel radius / mass in *Nm/kg* = *m/s^2*)
            double externalRelForce;        //!< External longitudinal relative force (force / mass in *N/kg* = *m/s^2*)
            double resistanceParameters[3]; //!< Resistance parameters to calculate the rel. resistance force f = a0 + a1 * v + a2 * v^2 (force / mass in *N/kg* = *m/s^2*)
        };

        /** A struct to store the vehicle state */
        struct State {
            double xPosition{};                                         //!< The actual absolute x-position
            double yPosition{};                                         //!< The actual absolute y-position
            double velocity{};                                          //!< The actual velocity
            double acceleration{};                                      //!< The actual acceleration
            double yawAngle{};                                          //!< The actual yaw angle
            double yawRate{};                                           //!< The actual yaw rate
            double distance{};                                          //!< The driven distance since the last reset
            IndicatorState indicatorState = IndicatorState::OFF;        //!< The actual indicator state
            ShifterPosition shifterPosition = ShifterPosition::PARK;    //!< The actual shifter position
            double indicatorTimer = INFINITY;                           //!< The indicator time until switch off
        };


        /**
         * Sets the shifter position
         * @param shifterPosition Shifter position
         */
        void setShifter(ShifterPosition shifterPosition) {

            using namespace std;

            // when reset flag is active allow everything
            if(_reset) {

                state.shifterPosition = shifterPosition;
                return;

            }

            // avoid switching to P when rolling
            if(shifterPosition == ShifterPosition::PARK && abs(state.velocity) > ZERO_VELOCITY)
                return;

            // avoid switching to R when moving forwards
            if(shifterPosition == ShifterPosition::REVERSE && state.velocity > ZERO_VELOCITY)
                return;

            // avoid switching to D when moving backwards
            if(shifterPosition == ShifterPosition::DRIVE && state.velocity < -ZERO_VELOCITY)
                return;

            // set shifter
            state.shifterPosition = shifterPosition;

        }


        /**
         * Switches the indicator on to the right
         * @param timeInterval The timer interval, the indicator shall be switched on
         */
        void indicateRight(double timeInterval = INFINITY) {
            state.indicatorState = IndicatorState::RIGHT;
            state.indicatorTimer = timeInterval;
        }


        /**
         * Switches the indicator on to the left
         * @param timeInterval The timer interval, the indicator shall be switched on
         */
        void indicateLeft(double timeInterval = INFINITY) {
            state.indicatorState = IndicatorState::LEFT;
            state.indicatorTimer = timeInterval;
        }


        /**
         * Switches the hazard indicator on
         */
        void hazard() {
            state.indicatorState = IndicatorState::HAZARD;
            state.indicatorTimer = INFINITY;
        }


        /**
         * Switches the indicators off
         */
        void indicatorOff() {
            state.indicatorState = IndicatorState::OFF;
            state.indicatorTimer = INFINITY;
        }


        /**
         * Resets the vehicle model
         */
        void reset() {

            state.distance = 0.0;
            state.indicatorTimer = INFINITY;

            _reset = true;

        }


        void step(double dt) {

            using namespace std;

            // undo reset
            _reset = false;

            // get state
            auto v0 = state.velocity;
            auto psi = state.yawAngle;

            // drive parameters
            auto power   = state.shifterPosition == ShifterPosition::REVERSE ? parameters.maxRelReverseDrivePower : parameters.maxRelDrivePower;
            auto torque  = parameters.maxRelDriveTorque;

            // inputs
            auto drive = state.shifterPosition == ShifterPosition::NEUTRAL ? 0.0 : range(input.drive, 0.0, 1.0);
            auto brake = range(input.brake, 0.0, 1.0);

            // calculate drive acceleration
            auto aDrive  = drive * range(power / abs(v0), -torque, torque);
            auto aExtern = parameters.externalRelForce;
            auto aBrake  = brake * parameters.maxRelBrakeTorque;
            auto aResist = parameters.resistanceParameters[0] + parameters.resistanceParameters[1] * v0 + parameters.resistanceParameters[2] * v0 * v0;

            // calculate longitudinal motion
            auto aAc = aDrive + aExtern;
            auto aBr = aBrake + aResist;

            // apply resistances when
            if(abs(v0) < ZERO_VELOCITY && aAc < 0.0)
                aBr = min(-aAc, aBr);   // set resistances against acc mit acc maximum
            else if(abs(v0) < ZERO_VELOCITY && aAc >= 0.0)
                aBr = -min(aAc, aBr);   // set resistances against acc mit acc maximum
            else if(v0 > 0.0)
                aBr = -aBr;             // resistances against vehicle motion

            // calculate until intermediate time point
            auto a = aBr + aAc;
            auto v = a * dt + v0;
            auto ds = 0.5 * a * dt * dt + v0 * dt;

            // speed cannot cross zero
            if((v0 > 0.0 && v < 0.0) || (v0 < 0.0 && v > 0.0) || (abs(v) < ZERO_VELOCITY))
                v = 0.0;

            // avoid motion when park brake is engaged
            if(state.shifterPosition == ShifterPosition::PARK) {
                a  = 0.0;
                v  = 0.0;
                ds = 0.0;
            }

            // steering model (linear bicycle model)
            auto dPsi = v * range(input.steering, -1.0, 1.0) * parameters.maxCurvature;

            // calculate 2D motion
            auto dx = cos(psi) * v - sin(psi) * dPsi * ds;
            auto dy = sin(psi) * v + cos(psi) * dPsi * ds;

            // update position and yaw angle
            state.xPosition += dx * dt;
            state.yPosition += dy * dt;
            state.yawAngle += dPsi * dt;

            // update other states
            state.distance += ds;
            state.acceleration = a;
            state.velocity = v;
            state.yawRate = dPsi;

            // indicator timer
            _indicatorTimer(dt);

        }


    protected:

        void _indicatorTimer(double dt) {

            // indicator timer
            if(!std::isinf(state.indicatorTimer)) {

                // switch off, when time interval has passed
                if(state.indicatorTimer <= EPS_SIM_TIME)
                    indicatorOff();

                // step down
                state.indicatorTimer -= dt;

            }

        }


        Parameters parameters{};
        ContinuousInput input;
        State state;

        bool _reset{};

    };

}

#endif //SIMCORE_VEHICLE_MODEL_H
