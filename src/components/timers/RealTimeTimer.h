//
// Created by klimke on 27.03.2019.
//

#ifndef SIMCORE_REALTIMETIMER_H
#define SIMCORE_REALTIMETIMER_H

#include <chrono>
#include <ctime>
#include <iostream>
#include <chrono>
#include <thread>
#include "BasicTimer.h"


class RealTimeTimer : public BasicTimer {

private:

    double _acceleration = 1.0;

    std::chrono::system_clock::time_point _refTime;
    unsigned long _steps = 0;


public:

    RealTimeTimer() = default;

    ~RealTimeTimer() override = default;


    void step() override {

        using namespace std::chrono;

        // create elapsed time
        auto elapsed = duration_cast<milliseconds>(system_clock::now() - _refTime);
        auto currTime = static_cast<double>(elapsed.count()) / 1000.0;

        // step increment
        _steps++;

        // wait until elapsed time
        auto nextTime = getTimeStepSize() * _steps;
        while (currTime < nextTime / _acceleration) {

            // wait a thousandth of a second
            std::this_thread::sleep_for(std::chrono::microseconds(100));

            // recalculate
            elapsed = duration_cast<milliseconds>(system_clock::now() - _refTime);
            currTime = static_cast<double>(elapsed.count()) / 1000.0;

        }

        // set current time
        setTime(nextTime);

    }


    void start() override {

        BasicTimer::start();
        _refTime = std::chrono::system_clock::now();

    }


    void reset() override {

        BasicTimer::reset();
        _steps = 0;

    }


    void setAcceleration(double acc) {

        _acceleration = acc;

    }


};


#endif //SIMCORE_REALTIMETIMER_H
