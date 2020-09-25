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
// Created by Jens Klimke on 2019-03-30
//

#ifndef SIMCORE_VALUEEXCEED_H
#define SIMCORE_VALUEEXCEED_H

#include "../IStopCondition.h"
#include "../IComponent.h"

namespace sim::value {

    template<typename T>
    class ValueExceed : public ::sim::IStopCondition, public ::sim::IComponent {

        IStopCondition::StopCode _mode = IStopCondition::StopCode::SIM_ENDED;

        const T *_pointer = nullptr;
        T _limit{};

    public:

        typedef IStopCondition::StopCode Mode;

        /**
         * Sets the pointer to the value to be checked, the limit of this value and optionally the stop mode
         * @param ptr The pointer to the value
         * @param limit The limit of the value
         * @param mode The mode to be set when limit is reached
         */
        void setPointerAndLimit(const T *ptr, const T &limit, Mode mode = Mode::SIM_ENDED) {

            _pointer = ptr;
            _limit = limit;

            // set mode
            setStopMode(mode);

        }


        /**
         * Sets the stop mode, to be set, when limit is reached
         * @param mode Stop mode
         */
        void setStopMode(Mode mode) {

            _mode = mode;

        }


        /**
         * Initialize: Resets the stop code
         * @param initTime Initialization time
         */
        void _init(double initTime) override {

            reset();

        }


        /**
         * Execution: Checks the value against the limit
         * @param simTime Simulation time
         */
        void _exec(double simTime) override {

            // the limit has been reached
            if (*_pointer > _limit)
                stop(_mode);

        }


        /**
         * Termination: does nothing
         * @param termTime Termination time
         */
        void _term(double termTime) override {}

    };

}

#endif //SIMCORE_VALUEEXCEED_H
