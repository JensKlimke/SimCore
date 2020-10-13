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
#include "../ISynchronized.h"
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


    class Simulation : public sim::Loop {

    protected:

        // managed elements
        std::unique_ptr<sim::ITimer> _timer{};
        std::vector<std::unique_ptr<sim::IComponent>> _managed{};

        // list of storable objects
        std::map<std::string, sim::storage::IStorable *> _stored{};

        // time step
        TimeStep timeStep{};

        // callbacks
        std::vector<std::function<void(const TimeStep &)>> _preInit{};
        std::vector<std::function<void(const TimeStep &)>> _preSteps{};
        std::vector<std::function<void(const TimeStep &)>> _postSteps{};
        std::vector<std::function<void(const TimeStep &)>> _preTerm{};


        void preInitialize(double t) {

            // set initialization time
            timeStep.initTime = t;

            // reset every other time elements
            timeStep.termTime = INFINITY;
            timeStep.time = 0.0;
            timeStep.deltaTime = 0.0;
            timeStep.steps = 0;

            // run init callback
            for (auto &cb : _preInit)
                cb(timeStep);

        }


        void preStep(double t, double dt) {

            // get time step size and save time
            timeStep.deltaTime = dt;
            timeStep.time = t;

            // run pre-steps
            for (auto &cb : _preSteps)
                cb(timeStep);

        }


        void postStep(double t, double dt) {

            // run post-steps
            for (auto &cb : _postSteps)
                cb(timeStep);

            // increment steps
            timeStep.steps++;

        }


        void preTerminate(double t) {

            // set termination time
            timeStep.termTime = t;

            // run terminate callback
            for (auto &cb : _preTerm)
                cb(timeStep);

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
            addComponent(comp);

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

            // reset loop
            reset();

            // delete lists
            _stored.clear();

            // create timer
            if (std::isinf(acceleration)) {

                // create and save basic timer
                auto timer = new sim::BasicTimer;

                // setup timer
                timer->setTimeStepSize(timeStepSize);
                timer->setStartTime(startTime);

                // set timer
                setTimer(timer);

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
                setTimer(timer);

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
            addStopCondition(stop);

        }


        /**
         * Returns the time structure
         * @return Time structure
         */
        [[nodiscard]] const TimeStep &time() const {

            return timeStep;

        }


        /**
         * Creates stop condition based on a given pointer (if value is exceed, the simulation stops)
         * @tparam VType Type of the value to be checked
         * @param stopValues A pair of time and limit
         * TODO: add stop mode
         */
        template<typename VType>
        void stopExceed(const std::pair<double, VType *> &stopValues) {

            // create stop condition
            auto b = createManagedComponent<sim::value::ValueExceed<VType>>();
            b->setPointerAndLimit(stopValues.first, stopValues.second);

            // add stop condition
            addStopCondition(b);

        }


    };

}

#endif // SIMCORE_SIMULATION_H
