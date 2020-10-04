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
// Created by Jens Klimke on 2019-03-20
//

#ifndef SIMCORE_TIME_IS_UP_H
#define SIMCORE_TIME_IS_UP_H

#include <cmath>
#include "../IStopCondition.h"
#include "../IComponent.h"

namespace sim {


    class TimeIsUp : public ::sim::IStopCondition, public ::sim::IComponent {


    private:


        double _stopTime = INFINITY;
        double _lastTime = INFINITY;


    public:

        /**
         * Default constructor
         */
        TimeIsUp() = default;


        /**
         * Method to set the stop time
         * @param stopTime Stop time
         */
        void setStopTime(double stopTime) {

            _stopTime = stopTime;

        }



    protected:

        /**
         * Initialize
         * @param initTime Initialization time
         */
        void _init(double initTime) override {

            reset();

        }


        /**
         * Step method
         * @param simTime Simulation time
         * @return Success flag
         */
        void _exec(double simTime) override {

            // set status to ended if time is reached
            if (simTime >= (_stopTime - EPS_SIM_TIME))
                end();

        }


        /**
         * Termination method
         * @param simTime Simulation time
         */
        void _term(double simTime) override {

            // set termination time
            _lastTime = simTime;

        }


    };

}


#endif // SIMCORE_TIME_IS_UP_H
