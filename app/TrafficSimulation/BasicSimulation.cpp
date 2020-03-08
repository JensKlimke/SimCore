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

#include "BasicSimulation.h"


BasicSimulation::~BasicSimulation() {

    destroy();

}


void BasicSimulation::destroy() {

    // timers
    delete timer;
    delete stopTimer;
    delete timeReporter;

    // logger
    delete plotLog;
    delete jsonLog;

    // delete stop conditions
    for(auto e : stopConditions)
        delete e;

}


void BasicSimulation::create(double endTime, double stepSize, bool realTime,
        const std::vector<std::pair<double*, double>> &stopValues) {

    // create timer
    timer = realTime ? new RealTimeTimer : new BasicTimer;

    // set timer
    timer->setTimeStepSize(stepSize);
    setTimer(timer);

    // stop condition (time)
    stopTimer = new TimeIsUp();
    stopTimer->setStopTime(endTime);

    // stop condition (distance) TODO: test
    for(auto e : stopValues) {

        // create stop condition
        auto b = new ValueExceed<double>();
        b->setValueAndLimit(e.first, e.second);

        // add stop condition
        stopConditions.push_back(b);
        addComponent(b);
        addStopCondition(b);

    }

    // add component and stop condition
    addComponent(stopTimer);
    addStopCondition(stopTimer);

    // only when real time, set time reporter
    if(realTime) {

        // set time reporter
        timeReporter = new TimeReporter();
        timeReporter->setTimeStepSize(1.0);

        // add component
        addComponent(timeReporter);

    }

}


void BasicSimulation::addLogValue(const std::string &key, const double *val) {

    if(jsonLog != nullptr)
        jsonLog->addValue(key, val);

}


PlotLogger * BasicSimulation::getLogger() {

    return plotLog;

}