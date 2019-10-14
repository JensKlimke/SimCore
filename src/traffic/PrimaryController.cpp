//
// Created by Jens Klimke on 2019-04-15.
//

#include <cmath>
#include "PrimaryController.h"


bool PrimaryController::step(double simTime) {

    auto dt = IComponent::timeStep(simTime);

    // calculate error (P)
    auto u = *_input.target - *_input.value;

    // calculate integral (I)
    _state.in += 0.5 * (u + _state.u) * dt;

    // calculate derivative (D)
    auto der = _reset ? 0.0 : (u - _state.u) / dt;
    _state.u = u;

    // initialize change of value
    double dy;

    // override when direct value is set, otherwise calculate controller change
    if(_input.direct != nullptr && !std::isinf(*_input.direct))
        dy = _parameters.d_P * (*_input.direct - *_state.y);
    else
        dy = _parameters.k_P * _state.u + _parameters.k_I * _state.in + _parameters.k_D * der;

    // limit change in controller
    dy = std::max(std::min(dy, _parameters.dyMax), -_parameters.dyMax);

    // integrate
    *_state.y = std::max(_parameters.range[0], std::min(_parameters.range[1], *_state.y + dy * dt));

    // unset reset flag
    _reset = false;

    return true;

}

void PrimaryController::reset() {

    _state.in = 0.0;
    _reset = true;

}


void PrimaryController::setVariables(double *value, double *target, double *output) {

    _input.value  = value;
    _input.target = target;
    _state.y      = output;

}


void PrimaryController::setParamters(double k_p, double k_i, double k_d) {

    _parameters.k_P = k_p;
    _parameters.k_I = k_i;
    _parameters.k_D = k_d;

}

void PrimaryController::initialize(double initTime) {

    initializeTimer(initTime);
    ISynchronized::initialize(initTime);

    reset();

}

std::vector<sim::data::IStorable::DataEntry> PrimaryController::getData(sim::data::IStorable::Context context) const {

    std::vector<DataEntry> ret;
    ret.reserve(2);

    switch(context) {
        case Context::PARAMETER:
            ADD(ret, k_P, _parameters);
            ADD(ret, k_I, _parameters);
            ADD(ret, k_D, _parameters);
            break;
        case Context::INPUT:
            ADD_PTR(ret, value,  _input);
            ADD_PTR(ret, target, _input);
            ADD_PTR(ret, direct, _input);
            break;
        case Context::STATE:
            ADD(ret,    in, _state);
            ADD(ret,     u, _state);
            ADD_PTR(ret, y, _state);
            break;
        default:
            break;
    }

    return ret;

}

