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
// Created by Jens Klimke on 2019-03-12
//

#ifndef SIMCORE_STOP_CONDITION_H
#define SIMCORE_STOP_CONDITION_H

#include <string>

namespace sim {


    class IStopCondition {

    public:

        enum class StopCode { NONE, OBJECTIVES_MISSED, OBJECTIVES_REACHED, SIM_ENDED };


        /**
         * Constructor for stop condition
         */
        IStopCondition() = default;


        /**
         * Default destructor
         */
        ~IStopCondition() = default;


        /**
         * Checks if the stop condition is set to a stopped state
         * @return Stop flag
         */
        [[nodiscard]] virtual bool hasStopped() const {

            return getCode() != StopCode::NONE;

        }


        /**
         * Resets the stop condition
         */
        virtual void reset() {

            _code = StopCode::NONE;

        }


        /**
         * Returns the stop code
         * @return Stop code
         */
        [[nodiscard]] StopCode getCode() const {

            return _code;

        }


    protected:

        // states
        sim::storage::Signal<StopCode> _code{this, "code", StopCode::NONE};


        /**
         * Call stop command with given code
         * @param code Code to be set
         */
        void stop(StopCode code) {

            _code = code;

        }


        /**
         * Call stop command (with objectives missed)
         */
        void failed() {

            _code = StopCode::OBJECTIVES_MISSED;

        }


        /**
         * Call stop command (with objectives reached)
         */
        void success() {

            _code = StopCode::OBJECTIVES_REACHED;

        }


        /**
         * Call stop command (with simulation ended regularly)
         */
        void end() {

            _code = StopCode::SIM_ENDED;

        }

    };

}


#endif //SIMCORE_STOP_CONDITION_H
