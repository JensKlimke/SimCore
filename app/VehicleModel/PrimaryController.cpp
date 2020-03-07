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
// Created by Jens Klimke on 2019-04-15.
//

#include "PrimaryController.h"


bool PrimaryController::step(double simTime) {

    auto dt = IComponent::timeStep(simTime);

    // calculate error (P)
    auto u = *_input.target - *_input.value;

    if(std::isinf(u))
        throw std::runtime_error("Input value is not finite.");

    // calculate integral (I)
    _state.in += (u + _state.u) * dt;

    // calculate derivative (D)
    auto der = _reset ? 0.0 : (u - _state.u) / dt;
    _state.u = u;

    // unset reset flag
    _reset = false;

    // calculate controller change
    auto dy = _parameters.k_P * _state.u + _parameters.k_I * _state.in + _parameters.k_D * der;

    // apply desired controller state
    if(_input.offset != nullptr && !std::isinf(*_input.offset)) {

        // change controller value
        dy = (*_input.offset - *_state.y) * 1.0;

        // reset controller
        reset();

    }

    // limit change of controller
    dy = std::min(_parameters.maxChange, std::max(-_parameters.maxChange, dy));

    // integrate
    *_state.y = std::max(_parameters.range[0], std::min(_parameters.range[1], *_state.y + dy * dt));

    return true;

}

void PrimaryController::reset() {

    _state.in = 0.0;
    _reset = true;

}


void PrimaryController::setVariables(double *value, double *target, double *output, double *offset) {

    _input.value = value;
    _input.target = target;
    _input.offset = offset;
    _state.y = output;

}


void PrimaryController::setParameters(double k_p, double k_i, double k_d) {

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

