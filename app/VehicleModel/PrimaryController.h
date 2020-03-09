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
// Created by Jens Klimke on 2019-03-22.
//

#ifndef SIMCORE_VM_PRIMARY_CONTROLLER_H
#define SIMCORE_VM_PRIMARY_CONTROLLER_H


#include <cmath>
#include <algorithm>
#include <simcore/Model.h>


class PrimaryController : public sim::Model {

public:

    struct Input {
        double *value;
        double *target;
        double *offset;
    };

    struct State {
        double in;
        double u;
        double *y;
    };

    struct Parameters {
        double k_P = 0.0;
        double k_I = 0.0;
        double k_D = 0.0;
        double range[2]  = {-1.0, 1.0};
        double maxChange = 1.0;
    };


private:

    bool _reset = false;

    Input _input{};
    State _state{};
    Parameters _parameters{};


public:

    /**
     * Initialization
     * @param initTime Initialize time
     */
    void initialize(double initTime) override;


    /**
     * Perform a controller step with the given time step size
     * @param dt Time step size
     */
    bool step(double simTime) override;


    /**
     * Reset the controller states (except the output value)
     */
    void reset();


    /**
     * Set the controller variables
     * @param value The actual value
     * @param target The desired value
     * @param output The output value
     * @param offset An offset added to the output
     */
    void setVariables(double *value, double *target, double *output, double *offset = nullptr);


    /**
     * Sets the parameters
     * @param k_p Proportional parameter
     * @param k_i Integral parameter
     * @param k_d Derivative parameter
     */
    void setParameters(double k_p, double k_i, double k_d);


    // generate accessor methods
    PARAM_ACCESS(Parameters, _parameters);
    INPUT_ACCESS(Input, _input);
    STATE_ACCESS(State, _state);


};

#endif // SIMCORE_VM_PRIMARY_CONTROLLER_H
