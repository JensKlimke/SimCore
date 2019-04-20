//
// Created by klimke on 27.03.2019.
//

#include "RealTimeTimer.h"
#include <ctime>
#include <thread>
#include <unistd.h>
#include <iostream>

void RealTimeTimer::step() {

    using namespace std::chrono;

    // create elapsed time
    auto elapsed = duration_cast<milliseconds>(system_clock::now() - _refTime);
    auto currTime = static_cast<double>(elapsed.count()) / 1000.0;

    // step increment
    _steps++;

    // wait until elapsed time
    auto nextTime = getTimeStepSize() * _steps;
    while (currTime < nextTime) {

        // wait a thousandth of a second
        usleep(100);

        // recalculate
        elapsed = duration_cast<milliseconds>(system_clock::now() - _refTime);
        currTime = static_cast<double>(elapsed.count()) / 1000.0;

    }

    // set current time
    setTime(nextTime);

}


void RealTimeTimer::start() {

    BasicTimer::start();
    _refTime = std::chrono::system_clock::now();

}


void RealTimeTimer::reset() {

    BasicTimer::reset();
    _steps = 0;

}
