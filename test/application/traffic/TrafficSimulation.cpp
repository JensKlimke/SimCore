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
 */

#include "TrafficSimulation.h"

TrafficSimulation::TrafficSimulation() = default;

TrafficSimulation::~TrafficSimulation() = default;

void TrafficSimulation::create(double endTime, double stepSize, bool realTime) {

    // create timer
    sim::BasicTimer *timer = realTime ? new sim::RealTimeTimer : new sim::BasicTimer;
    _timer.reset(timer);

    // set timer
    timer->setTimeStepSize(stepSize);
    _loop.setTimer(timer);

    // stop condition (time)
    auto stopTimer = new sim::TimeIsUp();
    stopTimer->setStopTime(endTime);

    // add component and stop condition
    _loop.addComponent(stopTimer);
    _loop.addStopCondition(stopTimer);

    // add to list
    _components.emplace_back(stopTimer);

    // only when real time, set time reporter
    if(realTime) {

        // set time reporter
        auto timeReporter = new sim::logging::TimeReporter();
        timeReporter->setTimeStepSize(1.0);

        // add component
        _loop.addComponent(timeReporter);

        // add to component list
        _components.emplace_back(timeReporter);

    }


}


void TrafficSimulation::stopExceed(const PtrValPairVec &stopValues) {

    // stop conditions
    for(auto e : stopValues) {

        // create stop condition
        auto b = new sim::value::ValueExceed<double>();
        b->setPointerAndLimit(e.first, e.second);

        // add stop condition
        _loop.addComponent(b);
        _loop.addStopCondition(b);

        // add to component list
        _components.emplace_back(b);

    }

}


void TrafficSimulation::addExternalComponent(sim::IComponent *comp) {

    // add the component to the loop
    _loop.addComponent(comp);

}


void TrafficSimulation::run() {

    // run the simulation
    _loop.run();

}