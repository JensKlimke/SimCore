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
// Created by Jens Klimke on $YEAR-$MONTH-23.
//


#include <simcore/framework/Simulation.h>
#include <simcore/timers/BasicTimer.h>
#include <simcore/timers/TimeIsUp.h>


void Simulation::component(sim::IComponent *component) {
    this->addComponent(component);
}

void Simulation::setInitialSpeed(double v) {
    _init_speed = v;
}

void Simulation::setDesiredSpeed(double v) {
    _desired_speed = v;
}


void Simulation::createTimer(double timeStepSize) {

    // create and setup timer
    auto timer = std::make_unique<sim::BasicTimer>();
    timer->setTimeStepSize(timeStepSize);

    // register
    this->setTimer(timer.get());

    // save timer
    _timer = std::move(timer);

}


void Simulation::setSimulationTime(double t) {

    // create and setup timer
    auto tiu = std::make_unique<sim::TimeIsUp>();
    tiu->setStopTime(t);

    // register timer
    this->addComponent(tiu.get());
    this->addStopCondition(tiu.get());

    // move to stop condition container
    _stopConditions.push_back(std::move(tiu));

}



double Simulation::execute() {

    // execute
    this->run();

    // return results
    return _desired_speed;

}