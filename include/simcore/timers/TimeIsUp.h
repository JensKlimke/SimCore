//
// Copyright (c) 2019 Jens Klimke <jens.klimke@rwth-aachen.de>. All rights reserved.
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

#ifndef SIMCORE_TIMEISUP_H
#define SIMCORE_TIMEISUP_H

#include "../IStopCondition.h"
#include "../IComponent.h"
#include <cmath>


#ifndef EPS_TIME
#define EPS_TIME 1e-12
#endif


class TimeIsUp : public ::sim::IStopCondition, public ::sim::IComponent {


private:


    double _stopTime = INFINITY;


public:

    /**
     * Default constructor
     */
    TimeIsUp() = default;


    /**
     * Initialize
     * @param initTime Initialization time
     */
    void initialize(double initTime) override {

        reset();

    }


    /**
     * Step method
     * @param simTime Simulation time
     * @return Success flag
     */
    bool step(double simTime) override {

        // set status to ended if time is reached
        if(simTime >= (_stopTime - EPS_TIME))
            end();

        return true;

    }


    /**
     * Termination method
     * @param simTime Simulation time
     */
    void terminate(double simTime) override {}


    /**
     * Method to set the stop time
     * @param stopTime Stop time
     */
    void setStopTime(double stopTime) {

        _stopTime = stopTime;

    }

};


#endif //SIMCORE_TIMEISUP_H
