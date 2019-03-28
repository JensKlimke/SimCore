//
// Created by Jens Klimke on 2019-03-22.
//

#ifndef SIMULATION_FRAMEWORK_PRIMARYCONTROLLER_H
#define SIMULATION_FRAMEWORK_PRIMARYCONTROLLER_H

#include <core/IModel.h>
#include <core/IStorable.h>
#include <core/IParameterizable.h>
#include <cmath>
#include <algorithm>

class PrimaryController : public sim::IModel, sim::IParameterizable, sim::IStorable {

public:

    struct Input {
        double *value;
        double *target;
    };

    struct State {
        double in;
        double u;
        double *y;
    };

    struct Parameters {
        double P;
        double I;
        double D;
        double range[2] = {-1.0, 1.0};
    };


private:

    bool       _reset = true;

    Input      _input;
    State      _state;
    Parameters _parameters;


public:


    void initialize(double initTime) override {

        initializeTimer(initTime);
        reset();

    }


    bool step(double simTime) override {

        // get time step
        double dt = timeStep(simTime);

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

        // success
        return true;

    }


    void terminate(double simTime) override {

    }


    void reset() {

        _state.in = 0.0;
        _reset    = true;

    }


    void setValues(double *value, double *target, double *output) {

        _input.value  = value;
        _input.target = target;
        _state.y      = output;

    }


    void setParamters(double p, double i, double d) {

        _parameters.P = p;
        _parameters.I = i;
        _parameters.D = d;

    }


    std::vector<DataEntry> getData(Context context) override {

        std::vector<DataEntry> ret;
        ret.reserve(6);

        switch(context) {
            case Context::PARAMETER:
                ADD(ret, P, _parameters);
                ADD(ret, I, _parameters);
                ADD(ret, D, _parameters);
                ADD(ret, range, _parameters);
                ret.push_back(DataEntry{"range.min", &_parameters.range[0]});
                ret.push_back(DataEntry{"range.max", &_parameters.range[1]});
                break;
            case Context::STATE:
                ADD(ret, in, _state);
                ADD(ret, u,  _state);
                ret.push_back(DataEntry{"y", _state.y});
                break;
            case Context::INPUT:
                ret.push_back(DataEntry{"value", _input.value});
                ret.push_back(DataEntry{"target", _input.target});
                break;
            default:
                break;
        }

        return ret;

    }


    PARAM_ACCESS(Parameters, _parameters)
    INPUT_ACCESS(Input,      _input)
    STATE_ACCESS(State,      _state)


};


#endif //SIMULATION_FRAMEWORK_PIDCONTROLLER_H
