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


#ifndef SIMCORE_BASIC_SIMULATION_H
#define SIMCORE_BASIC_SIMULATION_H

#include <string>
#include <memory>
#include <simcore/Loop.h>
#include <simcore/logging/TimeReporter.h>
#include <simcore/timers/BasicTimer.h>
#include <simcore/timers/RealTimeTimer.h>
#include <simcore/timers/TimeIsUp.h>
#include <simcore/value/ValueExceed.h>
#include <simcore/traffic/VehicleModel.h>


class TrafficSimulation {

    sim::Loop _loop;
    std::unique_ptr<sim::ITimer> _timer{};
    std::vector<std::unique_ptr<sim::IComponent>> _components{};


public:

    /**
     * Container to store double pointers and corresponding values
     */
    typedef std::vector<std::pair<double*, double>> PtrValPairVec;


    /**
     * Constructor
     */
    TrafficSimulation();


    /**
     * Destructor
     */
    virtual ~TrafficSimulation();


    /**
     * Creates a simulation
     * @param endTime End time of the simulation
     * @param stepSize Time step size of the simulation
     * @param realTime Real-time flag
     */
    void create(double endTime, double stepSize, bool realTime = false);


    /**
     * Add stop conditions
     * @param stopValues Values to be checked for excess (then simulation is stopped)
     */
    void stopExceed(const PtrValPairVec &stopValues = {});


    /**
     * Adds an unmanaged component (will not be deleted during destruction)
     * @param comp Component to be added
     */
    void addExternalComponent(sim::IComponent *comp);


    /**
     * Executes the simulation
     */
    void run();

};

#endif // SIMCORE_BASIC_SIMULATION_H
