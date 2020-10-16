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
#include <algorithm>
#include <SimCore.pb.h>
#include "utils/exceptions.h"
#include "timers/ITimer.h"
#include "IStopCondition.h"
#include "IComponent.h"


namespace sim {


    class Loop : public sim::storage::IStorable {

    public:

        /*!< Status of the loop */
        enum class Status {
            INITIALIZED, RUNNING, STOPPED
        };


        /**
         * Default constructor
         */
        Loop() {
            reset();
        }


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

            // check if component is already added
            if (std::find(_stopConditions.begin(), _stopConditions.end(), stop) != _stopConditions.end())
                throw sim::SetupException("Stop condition object has been already added.");

            _stopConditions.push_back(stop);

        }


        /**
         * Adds an model to the loop
         * @param name Name of the model
         * @param comp Model to be set
         */
        void addComponent(sim::IComponent *comp) {

            // check if component is already added
            if (std::find(_components.begin(), _components.end(), comp) != _components.end())
                throw sim::SetupException("Component has been already added.");

            _components.push_back(comp);

        }


        /**
         * Run simulation
         */
        void run() {

            if(_status == Status::RUNNING) {

                // execute when completely set up (and running)
                _executeDirectly();

            } else {

                // initialize components
                _initialize();

                // execute main loop
                _execute();

            }

            // terminate components
            _terminate();

        }


        /**
         * Abort the running simulation
         */
        void abort() {

            // check state
            if (_status != Status::RUNNING)
                throw ProcessException("Simulation is not running.");

            // set stop flag
            _stopFlag = true;

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


        /**
         * Stores the state to the given JSON object
         * @param obj JSON object
         */
        void toJSON(nlohmann::json &obj) const override {
            obj["status"] = _status;
            obj["stopFlag"] = _stopFlag;
        }


        /**
         * Sets the state given by the JSON object
         * @param obj JSON object
         */
        void fromJSON(const nlohmann::json &obj) override {
            obj["status"].get_to(_status);
            obj["stopFlag"].get_to(_stopFlag);
        }


        /**
         * Stores the state to the given protobuf object
         * @param obj Protobuf object
         */
        void toProtobuf(sim::protobuf::Loop &obj) const {
            obj.set_status(static_cast<sim::protobuf::Loop_Status>(_status));
            obj.set_stopflag(_stopFlag);
        }


        /**
         * Sets the state given by the protobuf object
         * @param obj Protobuf object
         */
        void fromProtobuf(const sim::protobuf::Loop &obj) {
            _status = static_cast<Status>(obj.status());
            _stopFlag = obj.stopflag();
        }


    protected:

        // states
        Status _status;
        bool _stopFlag;

        // elements
        std::vector<IComponent *> _components;
        std::vector<IStopCondition *> _stopConditions;
        ITimer *_timer;


        /**
         * Execute simulation
         */
        void _execute() {

            // check status
            if (_status != Status::INITIALIZED)
                throw ProcessException("Simulation must be initialized before running.");

            // set status
            _status = Status::RUNNING;

            // start timer
            _timer->start();

            // execute
            _executeDirectly();

        }


        /**
         * Execute simulation
         */
        void _executeDirectly() {

            // check status
            if(_status != Status::RUNNING)
                throw ProcessException("Simulation must be setup.");

            // run loop
            _loop();

            // stop timer
            _timer->stop();

        }


        /**
         * Runs the main loop of the simulation
         */
        void _loop() {

            // iterate while stop flag is not set
            while (!_stopFlag) {

                // iterate over components ...
                for (auto &m : _components) {

                    // ... and run component step
                    double t = _timer->time();
                    if (m->_execCondition(t))
                        m->_exec(t);

                }

                // iterate over stop conditions ...
                for (auto &sc : _stopConditions) {

                    // ... and check status
                    if (sc->hasStopped())
                        _stopFlag = true;

                }

                // time step
                if (!_stopFlag)
                    _timer->step();

            }

        }


        /**
         * Initialize simulation
         */
        void _initialize() {

            // check status
            if (_status != Status::STOPPED)
                throw ProcessException("Simulation must be stopped to be initialized.");

            // check timer
            if (_timer == nullptr)
                throw SetupException("A timer must be set.");

            // reset timer
            _timer->reset();


            // iterate over stop conditions ...
            for (auto &sc : _stopConditions) {

                // ... and reset
                sc->reset();

            }

            // iterate over components ...
            for (auto &m : _components) {

                // ... and initialize models
                m->_init(_timer->time());

            }

            // reset stop flag
            _stopFlag = false;

            // set status
            _status = Status::INITIALIZED;

        }


        /**
         * Terminate simulation
         */
        void _terminate() {


            // iterate over components ...
            for (auto &m : _components) {

                // ... and terminate
                m->_term(_timer->time());

            }

            // set status
            _status = Status::STOPPED;

        }


        void reset() {

            // states
            _status = Status::STOPPED;
            _stopFlag = true;

            // elements
            _components.clear();
            _stopConditions.clear();

            // timer
            _timer = nullptr;

        }


    };

}


#endif //SIMCORE_LOOP_H
