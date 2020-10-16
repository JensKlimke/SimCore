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
// Created by Jens Klimke on 2020-10-01.
//


#ifndef SIMCORE_SIMULATION_TEST_H
#define SIMCORE_SIMULATION_TEST_H

#include "Simulation.h"

namespace sim::testing {


    template<class T>
    class SimulationTest : public Simulation, public T {

    protected:

        // callbacks
        std::vector<std::function<void(const TimeStep &)>> _preInit{};
        std::vector<std::function<void(const TimeStep &)>> _preSteps{};
        std::vector<std::function<void(const TimeStep &)>> _postSteps{};
        std::vector<std::function<void(const TimeStep &)>> _preTerm{};


        void initialize(double t) override {

            // set initialization time
            _timeStep.initTime = t;

            // reset every other time elements
            _timeStep.termTime = INFINITY;
            _timeStep.time = 0.0;
            _timeStep.deltaTime = 0.0;
            _timeStep.steps = 0;

            // run init callback
            for (auto &cb : _preInit)
                cb(_timeStep);

            // run initialize of component
            T::initialze();

        }


        void step(double t, double dt) override {

            // get time step size and save time
            _timeStep.deltaTime = dt;
            _timeStep.time = t;

            // run pre-steps
            for (auto &cb : _preSteps)
                cb(_timeStep);

            // run step of main component
            T::step(t, dt);

            // run post-steps
            for (auto &cb : _postSteps)
                cb(_timeStep);

            // increment steps
            _timeStep.steps++;

        }


        void terminate(double t) override {

            // set termination time
            _timeStep.termTime = t;

            // run terminate callback
            for (auto &cb : _preTerm)
                cb(_timeStep);

            // run termination of component
            T::terminate(t);

        }


    };

}

#endif // SIMCORE_SIMULATION_TEST_H
