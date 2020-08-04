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


namespace sim {


    class IComponent {


        double _last_time_step{};


    public:


        /**
         * Default constructor
         */
        IComponent() = default;


        /**
         * Default destructor
         */
        virtual ~IComponent() = default;


        /**
         * Handles the initialization
         * @return Success flag
         */
        virtual void initialize(double initTime) = 0;


        /**
         * Handles a simulation step
         * @param simTime The current simulation time
         * @return Success flag
         */
        virtual bool step(double simTime) = 0;


        /**
         * Handles the termination
         * @return Success flag
         */
        virtual void terminate(double simTime) = 0;


    protected:



        /**
         * Resets the model timer
         * @param initTime Initial time
         */
        void initializeTimer(double initTime) {

            _last_time_step = initTime;

        }


        /**
        * Returns the time since the last time step was performed
        * @return Time passed
        */
        [[nodiscard]] double sinceLastTimeStep(double simTime) const {

            return simTime - _last_time_step;

        }


        /**
         * Executes a time step and return the time step size. Attention: only run once per simulation step
         * @param simTime Simulation time
         * @return Time step size
         */
        double timeStep(double simTime) {

            double dt = sinceLastTimeStep(simTime);
            _last_time_step = simTime;

            return dt;

        }


    };



} // namespace sim::model;


#endif // SIMCORE_I_COMPONENT_H
