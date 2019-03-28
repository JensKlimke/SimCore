//
// Created by Jens Klimke on 2019-03-19.
//


#include "BasicTimer.h"



void BasicTimer::step() {

    _time += _stepSize;

}

void BasicTimer::start() {

    // nothing to do

}

void BasicTimer::stop() {

}

double BasicTimer::time() const {

    return _time;

}

void BasicTimer::setTimeStepSize(double stepSize) {

    _stepSize = stepSize;

}


double BasicTimer::getTimeStepSize() const {

    return _stepSize;

}

void BasicTimer::reset() {

    _time = 0.0;

}

void BasicTimer::setTime(double time) {

    _time = time;

}
