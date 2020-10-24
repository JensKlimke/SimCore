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

        void initialize(double t) override {

            // pre initialization
            preInitialize(t);

            // run initialize of component
            T::initialize(t);

        }


        void step(double t, double dt) override {

            // pre step
            preStep(t, dt);

            // run step of main component
            T::step(t, dt);

            // post-step
            postStep(t, dt);

        }


        void terminate(double t) override {

            // pre terminate
            preTerminate(t);

            // run termination of component
            T::terminate(t);

        }


    };

}

#endif // SIMCORE_SIMULATION_TEST_H
