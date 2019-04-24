//
// Created by Jens Klimke on 2019-04-15.
//

#include "PrimaryController.h"

namespace agent_model {


    void PrimaryController::step(double dt) {

        // calculate error (P)
        auto u = *_input.target - *_input.value;

        // calculate integral (I)
        _state.in += (u + _state.u) * dt;

        // calculate derivative (D)
        auto der = _reset ? 0.0 : (u - _state.u) / dt;
        _state.u = u;

        // calculate controller change and integrate
        auto dy = _parameters.P * _state.u + _parameters.I * _state.in + _parameters.D * der;
        *_state.y = std::max(_parameters.range[0], std::min(_parameters.range[1], *_state.y + dy * dt));

        // unset reset flag
        _reset = false;

    }

    void PrimaryController::reset() {

        _state.in = 0.0;
        _reset = true;

    }


    void PrimaryController::setVariables(double *value, double *target, double *output) {

        _input.value = value;
        _input.target = target;
        _state.y = output;

    }


    void PrimaryController::setParamters(double k_p, double k_i, double k_d) {

        _parameters.P = k_p;
        _parameters.I = k_i;
        _parameters.D = k_d;

    }


}