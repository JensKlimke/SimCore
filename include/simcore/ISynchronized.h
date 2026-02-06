//
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
// Created by Jens Klimke on 2019-03-27
//

#ifndef SIMCORE_SYNCHRONIZED_H
#define SIMCORE_SYNCHRONIZED_H

#include "IComponent.h"

#ifndef EPS_SIM_TIME
#define EPS_SIM_TIME 1e-9
#endif

namespace sim {

    class ISynchronized : public sim::IComponent {

        double _timeStepSize{};
        double _deltaStartTime{};
        double _nextExecTime{};


    public:

        /**
         * Initializes the component. Increases the nexte execution time by the initialization time
         * @param initTime Initialization time
         */
        void initialize(double initTime) override {

            _nextExecTime = _deltaStartTime + initTime;
            initializeTimer(initTime);

        }


        /**
         * Performs a simulation step, when the next execution time is reached
         * @param simTime Current simulation time
         * @return Returns a flag whether the step is executed or skipped
         */
        bool step(double simTime) override {

            if(simTime + EPS_SIM_TIME >= _nextExecTime) {

                _nextExecTime += _timeStepSize;
                return true;

            }

            return false;

        }


        /**
         * Sets the time step size of the model
         * @param timeStepSize Time step size
         */
        void setTimeStepSize(double timeStepSize, double firstExecutionAfterStart = 0.0) {

            _timeStepSize = timeStepSize;
            _deltaStartTime = firstExecutionAfterStart;

        }

    };


} // namespace ::sim

#endif //SIMCORE_SYNCHRONIZED_H
