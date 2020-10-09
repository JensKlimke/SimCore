// Copyright (c) 2020 Jens Klimke (jens.klimke@rwth-aachen.de). All rights reserved.
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
// Created by Jens Klimke on 2020-10-01.
//


#ifndef SIMCORE_SIMULATION_H
#define SIMCORE_SIMULATION_H

#include <memory>
#include <vector>
#include <functional>
#include "../Loop.h"
#include "../Model.h"
#include "../timers/RealTimeTimer.h"
#include "../timers/TimeIsUp.h"
#include "../storage/IStorable.h"
#include "../logging/TimeReporter.h"
#include "../value/ValueExceed.h"

namespace sim::testing {


    struct TimeStep {
        double time;
        double deltaTime;
        double initTime;
        double termTime;
        unsigned long steps;
    };


    template<class T>
    class SimulationTest : public T {

    protected:

        // managed elements
        std::unique_ptr<sim::Loop> _loop{};
        std::unique_ptr<sim::ITimer> _timer{};
        std::vector<std::unique_ptr<sim::IComponent>> _managed{};

        // list of storable objects
        std::map<std::string, sim::storage::IStorable *> _stored{};

        // time step
        TimeStep _time{};

        // callbacks
        std::vector<std::function<void(const TimeStep &)>> _preSteps{};
        std::vector<std::function<void(const TimeStep &)>> _postSteps{};


        void initialize(double t) override {

            // call super init
            T::initialize(t);

            // set initialization time
            _time.initTime = t;

            // reset every other time elements
            _time.termTime = INFINITY;
            _time.time = 0.0;
            _time.deltaTime = 0.0;
            _time.steps = 0;

        }


        void step(double t, double dt) override {

            // get time step size and save time
            _time.deltaTime = dt;
            _time.time = t;

            // run pre-steps
            for (auto &ps : _preSteps)
                ps(_time);

            // run main step
            T::step(t, dt);

            // run post-steps
            for (auto &ps : _postSteps)
                ps(_time);

            // increment steps
            _time.steps++;

        }


        void terminate(double t) override {

            // set termination time
            _time.termTime = t;

            // terminate super
            T::terminate(t);

        }

    public:


        /**
         * Creates a component, registers it and takes responsibility to destroy
         * @tparam Comp Component type
         * @tparam Args Construction arguments types
         * @param args Arguments
         */
        template<class Comp, class ...Args>
        Comp *createManagedComponent(std::string &&name, Args &&...args) {

            // create component and add to component list
            auto comp = new Comp(std::forward<Args>(args)...);

            // add to lists
            _stored.emplace(name, comp);
            _managed.emplace_back(comp);

            // add component
            _loop->addComponent(comp);

            // return component
            return comp;

        }


        /**
         * Creates the simulation
         * @param endTime Time at which the simulation shall end
         * @param timeStepSize Time step size
         * @param startTime Time at which the simulation shall start, default: 0.0
         * @param acceleration The acceleration to real-time, the simulation shall be executed, default = INFINITY
         */
        void create(double endTime, double timeStepSize, double startTime = INFINITY, double acceleration = INFINITY) {

            // delete lists
            _stored.clear();

            // create loop
            _loop = std::make_unique<sim::Loop>();

            // create timer
            if (std::isinf(acceleration)) {

                // create and save basic timer
                auto timer = new sim::BasicTimer;

                // setup timer
                timer->setTimeStepSize(timeStepSize);
                timer->setStartTime(startTime);

                // set timer
                _loop->setTimer(timer);

                // set to timer unique pointer
                _timer.reset(timer);

            } else {

                // create real-time timer
                auto timer = new RealTimeTimer;

                // setup timer
                timer->setAcceleration(acceleration);
                timer->setTimeStepSize(timeStepSize);
                timer->setStartTime(startTime);

                // set timer
                _loop->setTimer(timer);

                // save timer
                _timer.reset(timer);

            }

            // when acceleration is low, set reporter to visualize time
            if (acceleration <= 10.0) {

                // set time reporter
                auto timeReporter = createManagedComponent<sim::logging::TimeReporter>("timeReporter");
                timeReporter->setTimeStepSize(1.0);

            }

            // stop condition (time)
            auto stop = createManagedComponent<sim::TimeIsUp>("stopTimer");
            stop->setStopTime(endTime);

            // add component as stop condition
            _loop->addStopCondition(stop);

            // add myself to the loop
            _loop->addComponent(this);

        }


        /**
         * Executes the simulation
         */
        void run() {

            // run loop
            _loop->run();

        }


        /**
         * Returns the time structure
         * @return Time structure
         */
        [[nodiscard]] const TimeStep &time() const {

            return _time;

        }


        /**
         * Creates stop condition based on a given pointer (if value is exceed, the simulation stops)
         * @tparam VType Type of the value to be checked
         * @param stopValues A pair of time and limit
         * TODO: add stop mode
         */
        template<typename VType>
        void stopExceed(const std::pair<double, VType*> &stopValues) {

            // create stop condition
            auto b = createManagedComponent<sim::value::ValueExceed<VType>>();
            b->setPointerAndLimit(stopValues.first, stopValues.second);

            // add stop condition
            addStopCondition(b);

        }


        /**
         * Adds a callback function to be executed before the execution step of this component is executed
         * @param fnc Callback function
         */
        void addPreCallback(std::function<void(const TimeStep &)> &&fnc) {

            // add to pre steps
            _preSteps.emplace_back(std::move(fnc));

        }


        /**
         * Adds a callback function to be executed before the execution step of this component is executed
         * @param fnc Callback function
         */
        void addPostCallback(std::function<void(const TimeStep &)> &&fnc) {

            // add to pre steps
            _postSteps.emplace_back(std::move(fnc));

        }

    };

}

#endif // SIMCORE_SIMULATION_H
