/*
 * Copyright 2020 Jens Klimke
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * author: Jens Klimke
 * date: 2020-03-07
 *
 */


#ifndef SIMCORE_TS_BASICSIMULATION_H
#define SIMCORE_TS_BASICSIMULATION_H

#include <simcore/Loop.h>
#include <simcore/data/JsonFileReporter.h>
#include <simcore/data/TimeReporter.h>
#include <simcore/timers/BasicTimer.h>
#include <simcore/timers/RealTimeTimer.h>
#include <simcore/timers/TimeIsUp.h>

class BasicSimulation : public sim::Loop {

protected:

    BasicTimer *timer = nullptr;
    TimeReporter *timeReporter = nullptr;
    TimeIsUp *stopTimer = nullptr;


public:

    /**
     * Constructor
     */
    BasicSimulation() = default;

    /**
     * Destructor
     */
    ~BasicSimulation() override;

    /**
     * Creates a simulation
     * @param endTime End time of the simulation
     * @param stepSize Time step size of the simulation
     */
    void create(double endTime, double stepSize, bool realTime = false);

};

#endif // SIMCORE_TS_BASICSIMULATION_H
