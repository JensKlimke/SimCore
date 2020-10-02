// Copyright (c) 2020 Jens Klimke.
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
// Created by Jens Klimke on 2020-09-30.
// Contributors:
//


#ifndef SIMTRAFFIC_VEHICLE_MODEL_ADAPTER_H
#define SIMTRAFFIC_VEHICLE_MODEL_ADAPTER_H

#include "VehicleModel.h"
#include <simcore/Model.h>

namespace sim::traffic {

    class VehicleModelAdapter : public VehicleModel, public sim::Model {

    public:

        /**
         * Constructor
         */
        VehicleModelAdapter() = default;


        /**
         * Destructor
         */
        ~VehicleModelAdapter() override = default;



    protected:


        /**
         * Initializes the vehicle model (resets the model state)
         * @param t Simulation time
         */
        void initialize(double t) override {

            // reset the model
            reset();

        }


        /**
         * Performs a step of the vehicle model
         * @param t Simulation time
         * @param dt Time step size
         */
        void step(double t, double dt) override {

            // performs the vehicle model step
            VehicleModel::step(dt);

        }


        /**
         * Terminates the vehicle model (in fact: does nothing)
         * @param t Simulation time
         */
        void terminate(double t) override {

            // nada

        }

    };

}

#endif // SIMTRAFFIC_VEHICLE_MODEL_ADAPTER_H
