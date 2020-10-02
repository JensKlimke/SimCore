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
#include <simcore/storage/Storable.h>


namespace sim::traffic {

    class VehicleModelAdapter : public VehicleModel, public sim::Model, public sim::storage::Storable {

    public:

        /**
         * Constructor
         */
        VehicleModelAdapter() = default;


        /**
         * Destructor
         */
        ~VehicleModelAdapter() override = default;


        /**
         * Registers the model to the data node
         * @param node Data node
         */
        void registerSignals(sim::storage::DataNode &node) override {

            // internal attributes
            REGISTER_SIGNAL_ALIAS(_indicatorTime, indicatorTime);
            REGISTER_SIGNAL_ALIAS(_reset, reset);

            // states
            REGISTER_SIGNAL_CONTEXT(velocity, state);
            REGISTER_SIGNAL_CONTEXT(acceleration, state);
            REGISTER_SIGNAL_CONTEXT(yawAngle, state);
            REGISTER_SIGNAL_CONTEXT(yawRate, state);
            REGISTER_SIGNAL_CONTEXT(curvature, state);
            REGISTER_SIGNAL_CONTEXT(distance, state);
            REGISTER_SIGNAL_CONTEXT(xPosition, state);
            REGISTER_SIGNAL_CONTEXT(yPosition, state);
            REGISTER_SIGNAL_CONTEXT(indicatorState, state);
            REGISTER_SIGNAL_CONTEXT(shifterPosition, state);

            // inputs
            REGISTER_SIGNAL_CONTEXT(drive, input);
            REGISTER_SIGNAL_CONTEXT(brake, input);
            REGISTER_SIGNAL_CONTEXT(steering, input);

            // parameters
            REGISTER_SIGNAL_CONTEXT(maxCurvature, parameters);
            REGISTER_SIGNAL_CONTEXT(maxRelDrivePower, parameters);
            REGISTER_SIGNAL_CONTEXT(maxRelReverseDrivePower, parameters);
            REGISTER_SIGNAL_CONTEXT(maxRelDriveTorque, parameters);
            REGISTER_SIGNAL_CONTEXT(maxRelBrakeTorque, parameters);
            REGISTER_SIGNAL_CONTEXT(externalRelForce, parameters);
            REGISTER_SIGNAL_CONTEXT(resistanceParameters[0], parameters);
            REGISTER_SIGNAL_CONTEXT(resistanceParameters[1], parameters);
            REGISTER_SIGNAL_CONTEXT(resistanceParameters[2], parameters);

        }


        void fromNode(const sim::storage::DataNode &node) override {

            // internal attributes
            WRITE_SIGNAL_ALIAS(_indicatorTime, indicatorTime);
            WRITE_SIGNAL_ALIAS(_reset, reset);

            // states
            WRITE_SIGNAL_CONTEXT(velocity, state);
            WRITE_SIGNAL_CONTEXT(acceleration, state);
            WRITE_SIGNAL_CONTEXT(yawAngle, state);
            WRITE_SIGNAL_CONTEXT(yawRate, state);
            WRITE_SIGNAL_CONTEXT(curvature, state);
            WRITE_SIGNAL_CONTEXT(distance, state);
            WRITE_SIGNAL_CONTEXT(xPosition, state);
            WRITE_SIGNAL_CONTEXT(yPosition, state);
            WRITE_SIGNAL_CONTEXT(indicatorState, state);
            WRITE_SIGNAL_CONTEXT(shifterPosition, state);

            // inputs
            WRITE_SIGNAL_CONTEXT(drive, input);
            WRITE_SIGNAL_CONTEXT(brake, input);
            WRITE_SIGNAL_CONTEXT(steering, input);

            // parameters
            WRITE_SIGNAL_CONTEXT(maxCurvature, parameters);
            WRITE_SIGNAL_CONTEXT(maxRelDrivePower, parameters);
            WRITE_SIGNAL_CONTEXT(maxRelReverseDrivePower, parameters);
            WRITE_SIGNAL_CONTEXT(maxRelDriveTorque, parameters);
            WRITE_SIGNAL_CONTEXT(maxRelBrakeTorque, parameters);
            WRITE_SIGNAL_CONTEXT(externalRelForce, parameters);
            WRITE_SIGNAL_CONTEXT(resistanceParameters[0], parameters);
            WRITE_SIGNAL_CONTEXT(resistanceParameters[1], parameters);
            WRITE_SIGNAL_CONTEXT(resistanceParameters[2], parameters);

        }


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
