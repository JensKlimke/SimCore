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

#ifndef SIMCORE_ICOMPONENT_H
#define SIMCORE_ICOMPONENT_H

#ifndef EPS_SIM_TIME
#define EPS_SIM_TIME 1e-9
#endif

#include <limits>
#include "exceptions.h"

namespace simcore {


    class IComponent {

    protected:

        friend class Loop;

        double _timeStepSize{};
        double _deltaStartTime = 0.0;

        double _lastTimeStep = 0.0;
        double _nextExecTime = 0.0;

        double _initTime{};
        double _termTime{};

        unsigned long _steps = 0;


        /**
         * Executes a time step and return the time step size. Attention: only run once per simulation step
         * @param simTime Simulation time
         * @return Time step size
         */
        double timeStep(double simTime) {

            // get time step size
            double dt = simTime - _lastTimeStep;

            // set next time step
            _lastTimeStep = simTime;

            // increment steps
            _steps++;

            // return time step size
            return dt;

        }


        /**
         * Initializes the component. Increases the nexte execution time by the initialization time
         * @param initTime Initialization time
         */
        virtual void _initialize(double initTime) {

            // reset steps
            _steps = 0;

            // set times
            _initTime = initTime;
            _termTime = -std::numeric_limits<double>::infinity();

            // set this execution time and next execution time
            _lastTimeStep = initTime;
            _nextExecTime = _deltaStartTime + initTime;

            // run initialization
            initialize(_initTime);

        }


        /**
         * Performs a simulation step, when the next execution time is reached
         * @param simTime Current simulation time
         * @return Returns a flag whether the step is executed or skipped
         */
        virtual void _step(double simTime) {

            if(simTime + EPS_SIM_TIME >= _nextExecTime) {

                // set next execution time
                _nextExecTime += _timeStepSize;

                // model step
                step(simTime, timeStep(simTime));

            }

        }


        /**
         * Handles the termination
         * @param simTime Simulation time at termination
         */
        virtual void _terminate(double simTime) {

            // set term time
            _termTime = simTime;

            // run terminations
            terminate(_termTime);

        }


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
         * A function to be overloaded
         * @param simTime Actual simulation time
         */
        virtual void step(double simTime) {

            throw StepNotImplemented("step(double simTime) called but not implemented");

        }


        /**
         * Executes a simulation step
         * @param simTime The actual simulation time
         * @param deltaTime The actual time step size
         */
        virtual void step(double simTime, double deltaTime) {

            // calls without time step size
            step(simTime);

        }


        /**
         * Handles the termination
         * @param simTime Simulation time at termination
         */
        virtual void terminate(double simTime) = 0;


        /**
         * Sets the time step size of the model
         * @param timeStepSize Time step size
         */
        void setTimeStepSize(double timeStepSize, double firstExecutionAfterStart = 0.0) {

            _timeStepSize = timeStepSize;
            _deltaStartTime = firstExecutionAfterStart;

        }


        /**
         * Returns the number steps performed since init
         * @return Number of steps
         */
        unsigned long getSteps() const {

            return _steps;

        }


    };



} // namespace simcore::model;


#endif //SIMCORE_IPLUGIN_H
