//
// Copyright (c) 2019-2020 Jens Klimke <jens.klimke@rwth-aachen.de>
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
// Created by Jens Klimke on 2019-03-27.
//

#ifndef SIMCORE_REALTIMETIMER_H
#define SIMCORE_REALTIMETIMER_H

#include <ctime>
#include <iostream>
#include <chrono>
#include <thread>
#include "BasicTimer.h"

namespace sim {

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
            auto nextTime = getTimeStepSize() * (double) _steps;
            while (currTime < nextTime / _acceleration) {

                // wait a thousandth of a second
                std::this_thread::sleep_for(std::chrono::microseconds(100));

                // recalculate
                elapsed = duration_cast<milliseconds>(system_clock::now() - _refTime);
                currTime = static_cast<double>(elapsed.count()) / 1000.0;

            }

            // set current time
            setTime(nextTime + getStartTime());

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

}


#endif //SIMCORE_REALTIMETIMER_H
