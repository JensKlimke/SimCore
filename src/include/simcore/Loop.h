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

#ifndef SIMCORE_LOOP_H
#define SIMCORE_LOOP_H

#include <vector>
#include "exceptions.h"
#include "ITimer.h"
#include "IStopCondition.h"
#include "IComponent.h"

namespace sim {

    class Loop {

    public:

        enum class Status { INITIALIZED, RUNNING, STOPPED };


    private:

        Status _status = Status::STOPPED;
        bool   _stop   = true;

        std::vector<IComponent*> _components{};
        std::vector<IStopCondition*> _stop_conditions{};

        ITimer *_timer = nullptr;


    public:


        /**
         * Default constructor
         */
        Loop() = default;


        /**
         * Default destructor
         */
        virtual ~Loop() = default;


        /**
         * Sets the timer of the loop
         * @param timer Timer to be set
         */
        void setTimer(ITimer *timer) {

            _timer = timer;

        }


        /**
         * Adds an stop condition to the loop
         * @param stop Stop condition
         */
        void addStopCondition(IStopCondition *stop) {

            _stop_conditions.push_back(stop);

        }


        /**
         * Adds an model to the loop
         * @param name Name of the model
         * @param comp Model to be set
         */
        void addComponent(sim::IComponent *comp) {

            _components.push_back(comp);

        }


        /**
         * Run simulation
         */
        void run() {

            // check status
            initialize();

            execute();

            // if loop ended, terminate regularly
            terminate();

        }


        /**
         * Abort the running simulation
         */
        void stop() {

            // check state
            if(_status != Status::RUNNING)
                throw ProcessException("Simulation is not running.");

            // set stop flag
            _stop = true;

            // set status
            _status = Status::STOPPED;

        }


        /**
         * Returns the status of the loops
         * @return Status
         */
        [[nodiscard]] Status getStatus() const {

            // status
            return _status;

        }


    protected:


        /**
         * Execute simulation
         */
        void execute() {

            // check status
            if(_status != Status::INITIALIZED)
                throw ProcessException("Simulation must be initialized before running.");

            // set status
            _status = Status::RUNNING;

            // start timer
            _timer->start();

            // iterate while stop flag is not set
            while(!_stop) {

                // iterate over components ...
                for (auto &m : _components) {

                    // ... and run component step
                    m->step(_timer->time());

                }

                // iterate over stop conditions ...
                for(auto &sc : _stop_conditions) {

                    // ... and check status
                    if (sc->hasStopped())
                        _stop = true;

                }

                // time step
                if(!_stop)
                    _timer->step();

            }

            // stop timer
            _timer->stop();

        }


        /**
         * Initialize simulation
         */
        void initialize() {

            // check status
            if(_status != Status::STOPPED)
                throw ProcessException("Simulation must be stopped to be initialized.");

            // check timer
            if(_timer == nullptr)
                throw ProcessException("A timer must be set.");

            // reset timer
            _timer->reset();


            // iterate over stop conditions ...
            for(auto &sc : _stop_conditions) {

                // ... and reset
                sc->reset();

            }

            // iterate over components ...
            for(auto &m : _components) {

                // ... and initialize models
                m->initialize(_timer->time());

            }

            // reset stop flag
            _stop = false;

            // set status
            _status = Status::INITIALIZED;

        }


        /**
         * Terminate simulation
         */
        void terminate() {


            // iterate over components ...
            for (auto &m : _components) {

                // ... and terminate
                m->terminate(_timer->time());

            }

            // set status
            _status = Status::STOPPED;

        }


    };


}


#endif //SIMCORE_LOOP_H
