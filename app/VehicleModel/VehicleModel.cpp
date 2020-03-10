// Copyright (c) 2019-2020 Jens Klimke <jens.klimke@rwth-aachen.de>
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
// Created by Jens Klimke on 2019-03-20.
//

#include "VehicleModel.h"
#include <algorithm>
#include <iostream>

#ifndef G_ACC
#define G_ACC 9.81
#endif

#ifndef RHO_AIR
#define RHO_AIR 1.2041
#endif



bool VehicleModel::step(double simTime) {

    // get step size
    auto dt = timeStep(simTime);

    // calculate wheel steer angle and curvature
    _state.delta = _param.steerTransmission * _input.steer;
    _state.kappa = _state.delta / _param.wheelBase;

    // calculate distance and velocity
    _state.ds = std::max(0.0, _state.v * dt + 0.5 * _state.a * dt * dt);
    _state.v = std::max(0.0, _state.v + _state.a * dt);

    // calculate position
    _state.s += _state.ds;
    _state.x += cos(_state.psi) * _state.ds;
    _state.y += sin(_state.psi) * _state.ds;

    // calculate yaw rate and yaw angle
    _state.dPsi = _state.v * _state.kappa;
    _state.psi += _state.dPsi * dt;

    // squared velocity
    auto v2 = _state.v * _state.v;

    // coefficients
    auto airCoeff = 0.5 * RHO_AIR * _param.cwA;
    auto rollCoeff = _param.rollCoefficient[0] + _param.rollCoefficient[1] * _state.v + _param.rollCoefficient[2] * v2;

    // limit power and gas pedal
    auto pedal = std::max(_input.pedal, 0.0) * (1.0 - _param.idle) + _param.idle;

    // calculate accelerations
    auto aGround = cos(_input.slope) * G_ACC;
    auto aAir   = airCoeff * v2 / _param.mass;
    auto aRoll  = rollCoeff * aGround;
    auto aSlope = sin(_input.slope) * G_ACC;
    auto aBrake = aGround * std::min(_input.pedal, 0.0);

    // calculate smooth force curve
    double _v0 = _param.powerMax / _param.forceMax;
    double _v1 = _param.lowSpeedBound;
    double F0 = _param.forceMax;
    double F1 = _param.powerMax / _v1;
    double _x = _state.v / _v1;

    // calculate drive force
    _state.force = _x < 1.0
            ? (F0 + _x * _x * (4.0 * F1 -  3.0 * F0) + _x * _x * _x * (2.0 * F0 - 3.0 * F1))    // low speed
            : _param.powerMax / _state.v;                                                       // high speed

    // calculate acceleration
    auto aAccel = pedal * _state.force / _param.mass;

    // calculate acceleration
    _state.a  = -aRoll - aAir - aSlope + aBrake + aAccel;
    _state.ay = _state.kappa * _state.v * _state.v;

    // unset acceleration, when standing
    if(_state.v == 0.0 && _state.a < 0.0)
        _state.a = 0.0;

    return true;

}

void VehicleModel::initialize(double initTime) {

    IComponent::initializeTimer(initTime);

}

void VehicleModel::terminate(double simTime) {

    Model::terminate(simTime);

}