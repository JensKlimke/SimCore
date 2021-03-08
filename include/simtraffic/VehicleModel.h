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

class VehicleModel {

protected:

    double _time{};

public:

    struct {
        double maximumRelativePower = 1e2;
        double maximumAcceleration = 10.0;
        double maximumDeceleration = -10.0;
        double maximumVelocity = 70.0;
        double idleAcceleration = 0.2;
    } parameters;

    struct {
        double acceleration{};
        double velocity{};
        double distance{};
    } state;

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

    double idle(double velocity) const {

        // calculate parameters
        auto p = -(parameters.maximumRelativePower / parameters.maximumVelocity + parameters.idleAcceleration)
                / (parameters.maximumVelocity * parameters.maximumVelocity);

        // calculate current maximum force
        return p * velocity * velocity + parameters.idleAcceleration;

    }


    /**
     * Returns the relative power (per velocity and mass)
     * @param velocity Velocity state
     * @return Relative power
     */
    double fullThrottle(double velocity) const {

        // get min
        return parameters.maximumRelativePower / velocity;

    }


    /**
     * Returns the acceleration resulting from the pedal value
     * @param velocity Velocity state
     * @param pedal Pedal value
     * @return The resulting acceleration
     */
    double forwards(double velocity, double pedal) const {

        // get max and min acceleration
        auto aMin = idle(velocity);
        auto aMax = std::min(fullThrottle(velocity) + aMin, parameters.maximumAcceleration);

        // limit pedal value
        auto drive = std::min(1.0, std::max(0.0,  pedal));
        auto brake = std::min(1.0, std::max(0.0, -pedal));

        // calculate acceleration
        if(pedal >= 0.0)
            return drive * aMax + (1.0 - drive) * aMin;
        else
            return (1.0 - brake) * aMin + brake * parameters.maximumDeceleration;

    }


    /**
     * Returns the pedal value needed to accelerate as desired
     * @param velocity Velocity state
     * @param acceleration Desired acceleration
     * @return The required pedal value
     */
    double backwards(double velocity, double acceleration) const {

        // calculate acceleration points
        auto aMin = idle(velocity);
        auto aMax = std::min(fullThrottle(velocity) + aMin, parameters.maximumAcceleration);
        auto aBr  = parameters.maximumDeceleration;

        // calculate pedals
        if(aMin < acceleration)
            return (acceleration - aMin) / (aMax - aMin);
        else
            return (aMin - acceleration) / (aBr - aMin);

    }


    /**
     * Calculates a forward step of the vehicle model
     * @param time Actual time
     */
    void step(double time) {

        // calculate acceleration
        state.acceleration = forwards(state.velocity, input.pedal);
        state.velocity += state.acceleration * (time  - _time);

        // limit velocity
        state.velocity = state.velocity < 0.0 ? 0.0 : state.velocity;

        // set time
        _time = time;

    }


};


#endif // SIMCORE_VEHICLE_MODEL_H
