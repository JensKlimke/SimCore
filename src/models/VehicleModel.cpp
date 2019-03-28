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

    // copy data
    _stateIn = _state;

    // get step size
    auto dt = timeStep(simTime);

    // calculate yaw rate
    _state.dPsi = _stateIn.v * (_param.steerTransmission * _input.steer) / _param.wheelBase;
    _state.psi += _state.dPsi * dt;

    // calculate distance and velocity
    _state.ds = std::max(0.0, _stateIn.v * dt + 0.5 * _stateIn.a * dt * dt);
    _state.v = std::max(0.0, _stateIn.v + _stateIn.a * dt);

    // calculate distance
    _state.s += _state.ds;
    _state.xy[0] += cos(_stateIn.psi) * _state.ds;
    _state.xy[1] += sin(_stateIn.psi) * _state.ds;

    // squared velocity
    auto v2 = _state.v * _state.v;

    // coefficients
    auto airCoeff = 0.5 * RHO_AIR * _param.cwA;
    auto rollCoeff = std::min(1.0, std::max(-1.0, _state.v))
         * (_param.rollCoefficient[0] + _param.rollCoefficient[1] * _state.v + _param.rollCoefficient[2] * v2);

    // calculate accelerations
    auto aGround = cos(_input.slope) * G_ACC;
    auto aAir   = 0.5 * airCoeff * v2 / _param.mass;
    auto aRoll  = rollCoeff * aGround;
    auto aSlope = sin(_input.slope) * G_ACC;

    // calculate acceleration
    _state.a = -aRoll - aAir - aSlope                          // resistance
         + aGround * std::min(_input.pedal, 0.0)               // braking
         + std::max(_input.pedal, 0.0) * _param.power          // acceleration
           / (std::max(fabs(_state.v), 1.0) * _param.mass);

    return true;

}

void VehicleModel::initialize(double initTime) {

    IModel::initializeTimer(initTime);

}

void VehicleModel::terminate(double simTime) {

}
