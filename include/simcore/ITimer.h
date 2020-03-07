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
// Created by Jens Klimke on 2019-03-19.
//

#ifndef SIMCORE_ITIMER_H
#define SIMCORE_ITIMER_H

namespace sim {

    class ITimer {

    public:


        /**
         * Default constructor
         */
        ITimer() = default;


        /**
         * Default deconstructor
         */
        virtual ~ITimer() = default;


        /**
         * Performs a step
         */
        virtual void step() = 0;


        /**
         * Resets the timer
         */
        virtual void start() = 0;


        /**
         * Stops the timer
         */
        virtual void stop() = 0;


        /**
         * Returns the current simulation time
         * @return
         */
        virtual double time() const = 0;


        /**
         * Resets the timer
         */
        virtual void reset() = 0;

    };

} // namespace ::sim


#endif //SIMCORE_TIMER_H
