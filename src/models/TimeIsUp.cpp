//
// Created by Jens Klimke on 2019-03-20.
//

#include "TimeIsUp.h"
#include <iostream>


#ifndef EPS_TIME
#define EPS_TIME 1e-9
#endif


void TimeIsUp::initialize(double initTime) {

    reset();

}

bool TimeIsUp::step(double simTime) {

    // set status to ended if time is reached
    if(simTime >= (_stopTime - EPS_TIME))
        end();

    return true;

}

void TimeIsUp::terminate(double simTime) {

    // Nothing to do

}


void TimeIsUp::setStopTime(double stopTime) {

    _stopTime = stopTime;

}