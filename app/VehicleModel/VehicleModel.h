// Copyright (c) 2019 Institute for Automotive Engineering (ika), RWTH Aachen University. All rights reserved.
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
// Created by Jens Klimke on 2019-03-17.
//

#ifndef SIMCORE_VEHICLEMODEL_H
#define SIMCORE_VEHICLEMODEL_H


#include <simcore/Model.h>
#include <cmath>


class VehicleModel : public ::sim::Model {

public:

    struct Parameters {
        double steerTransmission  = 0.5;
        double wheelBase          = 3.0;
        double cwA                = 0.6;
        double mass               = 1.5e3;
        double powerMax           = 1.0e5;
        double forceMax           = 1.5e4;
        double idle               = 0.1;
        double rollCoefficient[3] = {4.0 * 9.91e-3, 4.0 * 1.95e-5, 4.0 * 1.76e-9};
        double size[2]            = {5.0, 2.2};
        double driverPos[2]       = {0.5, 0.5};
    };


    struct Input {
        double steer = 0.0;
        double pedal = 0.0;
        double slope = 0.0;
        int    gear  = 1;   // TODO: implement this correctly
    };


    struct State {
        double s;
        double ds;
        double psi;
        double dPsi;
        double kappa;
        double delta;
        double x;
        double y;
        double v;
        double a;
        double ay;
    };


private:

    Parameters _param;
    State      _state{};
    Input      _input;

public:

    VehicleModel() = default;
    ~VehicleModel() override = default;

    /**
     * Initializes the vehicle model
     * @param initTime Initialization time
     */
    void initialize(double initTime) override;


    /**
     * Executes a model step
     * @param simTime Simulation time
     * @return Returns the success flag
     */
    bool step(double simTime) override;

    /**
     * Terminates the vehicle model
     * @param simTime Simulation time
     */
    void terminate(double simTime) override;

    /**
     * default accessors
     */
    PARAM_ACCESS(Parameters, _param)
    INPUT_ACCESS(Input,      _input)
    STATE_ACCESS(State,      _state)

    /**
     * Data accessor
     * @param context Context of the data (e.g. input, state, parameters)
     * @return Returns nothing in the moment (TODO:)
     */
    std::vector<DataEntry> getData(Context context) const override {
        return {};
    }


};


#endif //SIMCORE_VEHICLEMODEL_H
