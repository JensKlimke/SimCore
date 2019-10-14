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

#ifndef EPS_VELOCITY
#define EPS_VELOCITY 1e-6
#endif

template <typename T> inline int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}


bool VehicleModel::step(double simTime) {

    using namespace std;

    // get step size
    auto dt = timeStep(simTime);

    // get parameters
    auto rc = _param.rollCoefficient;
    auto pc = _param.powerCoefficient;

    // save yaw angle for later
    double psi = _state.psi;

    // get velocity, squared velocity and sign of velocity
    auto v  = _state.v;
    auto v1 = fabs(v);
    auto v2 = v * v;
    auto dir = sgn(v);

    if(v < 1.0 && _input.pedal >= 0.0)
        std::cout << "STOP" << std::endl;

    // calculate accelerations
    auto aSlope = sin(_input.slope) * G_ACC;
    auto fAir = dir * 0.5 * RHO_AIR * _param.cwA * v2;
    auto aRoll = dir * (rc[0] + rc[1] * v1 + rc[2] * v2) * cos(_input.slope) * G_ACC;

    // calculate power
    auto x = max(0.0, min(1.0, _state.v / _param.lowSpeedThreshold));
    auto power = _param.maxPower * (pc[0] + (pc[1] + (pc[2] +  pc[3] * x) * x) * x);

    // calculate forces
    _state.FBody  = -(aRoll + aSlope) * _param.mass - fAir;
    _state.FAccel = min(G_ACC * _param.mass, max(_input.pedal, 0.0) * power / max(0.1, v1));
    _state.FBrake = dir * G_ACC * min(_input.pedal, 0.0) * _param.mass; // TODO: Limit to slope

    // calculate yaw rate
    _state.dPsi = _state.v * (_param.steerTransmission * _input.steer) / _param.wheelBase;
    _state.psi += _state.dPsi * dt;

    // get acceleration
    _state.a = (_state.FBody + _state.FAccel + _state.FBrake) / _param.mass;

    // calculate time to standstill
    auto dtt = -_state.v / _state.a;
    if(dtt > 0 && dtt < dt) {

        // calculate distance till standstill, set velocity to zero
        _state.ds = _state.v * dtt + 0.5 * _state.a * dtt * dtt;
        _state.v  = 0.0;

    } else {

        // calculate distance and velocity
        _state.ds = _state.v * dt + 0.5 * _state.a * dt * dt;
        _state.v = _state.v + _state.a * dt;

    }

    // calculate distance
    _state.s += _state.ds;
    _state.xy[0] += cos(psi) * _state.ds;
    _state.xy[1] += sin(psi) * _state.ds;

    return true;

}

void VehicleModel::initialize(double initTime) {

    IComponent::initializeTimer(initTime);

}

void VehicleModel::terminate(double) {

}
