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


#ifndef SIMCORE_SIMULATION_H
#define SIMCORE_SIMULATION_H

#include <vector>
#include <functional>
#include "Loop.h"
#include "Model.h"

namespace sim {

    template<class T>
    class Simulation : public Loop, public T, protected Model {

    public:

        void create() {

            // TODO: create

            // add myself to the loop
            this->addComponent(this);

        }


    protected:

        double _time{};
        double _deltaTime{};
        unsigned long _steps{};

        std::vector<std::function<void(const Simulation *)>> _preSteps{};
        std::vector<std::function<void(const Simulation *)>> _postSteps{};


        void initialize(double t) override {

            _time = 0.0;
            _deltaTime = 0.0;
            _steps = 0;

        }

        void _exec(double t) override {

            // get time step size and save time
            _deltaTime = t - _time;
            _time = t;

            // run pre-steps
            for (auto &ps : _preSteps)
                ps(this);

            // run main step
            step(t, timeStep(t));;

            // run post-steps
            for (auto &ps : _preSteps)
                ps(this);

            // increment steps
            _steps++;

        }

        void terminate(double t) override {

        }

    };

}

#endif // SIMCORE_SIMULATION_H
