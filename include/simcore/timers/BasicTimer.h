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
// Created by Jens Klimke on 2019-03-19
//

#ifndef SIMCORE_BASIC_TIMER_H
#define SIMCORE_BASIC_TIMER_H

#include "ITimer.h"


namespace sim {

    class BasicTimer : public ::sim::ITimer {

    public:


        /**
         * Default constructor
         */
        BasicTimer() = default;


        /**
         * Default destructor
         */
        ~BasicTimer() override = default;


        /**
         * Performs a time step
         */
        void step() override {

            _time += _stepSize;

        }


        /**
         * Starts the timer
         */
        void start() override {}


        /**
         * Stops the timer
         */
        void stop() override {}


        /**
         * Returns the actual time
         * @return The actual time
         */
        [[nodiscard]] double time() const override {

            return static_cast<double>(_time);

        }


        /**
         * Resets the timer
         */
        void reset() override {

            _time = _startTime;

        }


        /**
         * Sets the time step size of the timer
         * @param stepSize Time step size
         */
        void setTimeStepSize(double stepSize) {

            _stepSize = stepSize;

        }


        /**
         * Sets the start time of the timer
         * @param startTime Start time to be set
         */
        void setStartTime(double startTime) {

            _startTime = startTime;

        }


        /**
         * Returns the step size of the timer
         * @return Step size of the timer
         */
        [[nodiscard]] double getTimeStepSize() const {

            return static_cast<double>(_stepSize);

        }


        /**
         * Returns the start time
         * @return Start time
         */
        [[nodiscard]] double getStartTime() const {

            return static_cast<double>(_startTime);

        }


    protected:

        // states
        double _time;
        double _stepSize;
        double _startTime = 0.0;


        /**
         * Sets the current time
         * @param time Time to be set
         */
        void setTime(double time) {

            _time = time;

        }

    };

}


#endif // SIMCORE_BASIC_TIMER_H
