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
// Created by Jens Klimke on 2020-08-21.
//

#ifndef SIMCORE_SYNCHRONIZED_TIMER_H
#define SIMCORE_SYNCHRONIZED_TIMER_H

#include <thread>
#include <chrono>
#include <iostream>
#include "BasicTimer.h"

namespace sim {

    class SynchronizedTimer : public BasicTimer {

    protected:

        // states
        double _acceleration = 1.0; //!< Acceleration factor of the timer
        double _startRefTime;       //!< Start point in reference time
        double _refTime;            //!< Actual reference time of the synchronization system
        unsigned long _steps;       //!< Steps performed


    public:

        /**
         * Constructor
         */
        SynchronizedTimer() = default;


        /**
         * Destructor
         */
        ~SynchronizedTimer() override = default;


        /**
         * Executes a sync time step
         */
        void step() override {

            // get next time
            auto nextTime = getTimeStepSize() * static_cast<double>(static_cast<unsigned long>(++_steps));

            // wait until time has been elapsed
            double elapsed;
            while ((elapsed = getElapsedTime()) < nextTime)
                std::this_thread::sleep_for(std::chrono::microseconds(10));

            // set current time
            setTime(elapsed + getStartTime());

        }


        /**
         * Returns the (accelerated) time elapsed since start
         * @return Elapsed time
         */
        [[nodiscard]] double getElapsedTime() const {

            // calculate accelerated elapsed time
            return (_refTime - _startRefTime) * _acceleration;

        }


        /**
         * Sets the actual reference time from the synchronization time system
         * @param refTime Reference time
         */
        void setReferenceTime(double refTime) {

            // update the reference time
            _refTime = refTime;

        }


        /**
         * Start the timer
         */
        void start() override {

            // set the start time
            _startRefTime = _refTime;

            // reset super
            BasicTimer::start();

        }


        /**
         * Resets the timer
         */
        void reset() override {

            // reset steps
            _steps = 0;
            _refTime = 0.0;

            // reset super
            BasicTimer::reset();

        }


        /**
         * Sets the acceleration factor. 1 means, that the sync time speed is used as simulation time speed. Factor x
         * means, that the time speed of simulation is x times faster, than the sync time.
         * @param acc Acceleration factor
         */
        void setAcceleration(double acc) {

            // set acceleration
            _acceleration = acc;

        }

    };

}


#endif // SIMCORE_SYNCHRONIZED_TIMER_H
