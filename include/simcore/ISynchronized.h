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
// Created by Jens Klimke on 2019-03-27
//

#ifndef SIMCORE_SYNCHRONIZED_H
#define SIMCORE_SYNCHRONIZED_H

#include "IComponent.h"


namespace sim {

    class ISynchronized : public sim::IComponent {

        double _timeStepSize;
        double _deltaStartTime;
        double _nextExecTime;
        double _last_time_step{};


    public:


        /**
         * Sets the time step size of the model
         * @param timeStepSize Time step size
         */
        void setTimeStepSize(double timeStepSize) {

            _timeStepSize = timeStepSize;

        }


        /**
         * Sets the time (relative to first time step) when the component shall be executed the first time.
         * The absolute first execution time is calculated during initialization. The relative time is stored
         * and valid for each simulation loop.
         * @param firstExecutionAfterStart The first relative execution time
         */
        void setStartTime(double firstExecutionAfterStart) {

            _deltaStartTime = firstExecutionAfterStart;

        }


    protected:

        /**
         * Executes a time step and return the time step size. Attention: only run once per simulation step
         * @param simTime Simulation time
         * @return Time step size
         */
        double timeStep(double simTime) {

            // get time step size
            double dt = sinceLastTimeStep(simTime);
            _last_time_step = simTime;

            // update time
            _nextExecTime += _timeStepSize;

            return dt;

        }


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
         * Initializes the model
         * @param t Initialization time
         */
        virtual void initialize(double t) = 0;


        /**
         * Performs a main step
         * @param t Actual simulation time
         * @param dt Time step size since last execution step
         */
        virtual void step(double t, double dt) = 0;


        /**
         *
         * @param t Termination time
         */
        virtual void terminate(double t) = 0;



    private:

        /**
         * Initializes the component. Increases the next execution time by the initialization time
         * @param initTime Initialization time
         */
        void _init(double initTime) override {

            // create timer
            _nextExecTime = _deltaStartTime + initTime;
            initializeTimer(initTime);

            // initialize model
            initialize(initTime);

        }


        /**
         * Returns whether next execution step is reached
         * @param simTime Simulation time
         * @return Flag
         */
        bool _execCondition(double simTime) override {

            // check if next execution time is reached
            return simTime + EPS_SIM_TIME >= _nextExecTime;

        }


        /**
         * Performs a simulation step, when the next execution time is reached
         * @param simTime Current simulation time
         * @return Returns a flag whether the step is executed or skipped
         */
        void _exec(double simTime) override {

            // model step
            step(simTime, timeStep(simTime));

        }


        /**
         * Terminate the component
         * @param termTime Termination time
         */
        void _term(double termTime) override {

            // initialize model
            terminate(termTime);

        }

    };


} // namespace ::sim

#endif //SIMCORE_SYNCHRONIZED_H
