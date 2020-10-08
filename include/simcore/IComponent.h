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

#ifndef SIMCORE_I_COMPONENT_H
#define SIMCORE_I_COMPONENT_H

#ifndef EPS_SIM_TIME
#define EPS_SIM_TIME 1e-9
#endif

#include "storage/Signal.h"

namespace sim {

    class Loop;

    class IComponent {

    public:

        /*!< Allow Loop class access to methods */
        friend class Loop;


        /**
         * Default constructor
         */
        IComponent() = default;


        /**
         * Default destructor
         */
        virtual ~IComponent() = default;


    protected:

        /**
         * Handles the initialization
         */
        virtual void _init(double initTime) = 0;


        /**
         * Checks the condition to perform the execution step
         * Default is true. Override to implement condition.
         * @param simTime Simulation time
         * @return Flag to indicate whether the execution shall be done in this step
         */
        virtual bool _execCondition(double simTime) {

            // default is true
            return true;

        }


        /**
         * Handles a simulation step
         * @param simTime The current simulation time
         */
        virtual void _exec(double simTime) = 0;


        /**
         * Handles the termination
         */
        virtual void _term(double simTime) = 0;


    };



} // namespace sim


#endif // SIMCORE_I_COMPONENT_H
