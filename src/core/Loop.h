//
// Created by Jens Klimke on 2019-03-19.
//

#ifndef SIMCORE_LOOP_H
#define SIMCORE_LOOP_H

#include "Exceptions.h"
#include <vector>


namespace sim {

    // pre-declare classes for usage in Loop
    class IComponent;
    class IStopCondition;
    class ITimer;


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
         * Default deconstructor
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

        }


        /**
         * Returns the status of the loops
         * @return Status
         */
        Status getStatus() const {

            // status
            return _status;

        }


    private:


        /**
         * Initialize simulation
         */
        void initialize() {

            // check status
            if(_status != Status::STOPPED)
                throw ProcessException("Simulation must be stopped to be initialized.");

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


}; // namespace ::sim


#endif //SIMCORE_LOOP_H
