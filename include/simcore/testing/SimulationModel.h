// Copyright (c) 2020 Jens Klimke (jens.klimke@rwth-aachen.de). All rights reserved.
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
// Created by Jens Klimke on 2020-10-13.
//


#ifndef SIMCORE_SIMULATION_MODEL_H
#define SIMCORE_SIMULATION_MODEL_H

#include "Simulation.h"

namespace sim::testing {

    class SimulationModel : public Simulation, public ISynchronized {

        std::function<void(const TimeStep &timeStep)> _callback{};

    public:

        void setCallback(std::function<void(const TimeStep &timeStep)> &&fnc) {

            _callback = std::move(fnc);

        }

        void create(double endTime, double timeStepSize, double startTime = 0.0, double acceleration = INFINITY) {

            // create simulation
            Simulation::create(endTime, timeStepSize, startTime, acceleration);

            // add this as component
            addComponent(this);

            // set time step size
            setTimeStepSize(timeStepSize);

        }


    protected:


        void initialize(double t) override {

            // pre-init
            preInitialize(t);

        }

        void step(double t, double dt) override {

            // pre-step
            preStep(t, dt);

            // a callback
            if(_callback)
                _callback(_timeStep);

            // post-step
            postStep(t, dt);

        }

        void terminate(double t) override {

            // pre term
            preTerminate(t);

        }

    };

}

#endif // SIMCORE_SIMULATION_MODEL_H
