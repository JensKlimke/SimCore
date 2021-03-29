/*
 * Copyright 2020 Jens Klimke
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * author: Jens Klimke
 * date: 2020-03-07
 *
 */


#ifndef SIMCORE_BASIC_SIMULATION_H
#define SIMCORE_BASIC_SIMULATION_H

#include <memory>
#include <string>
#include <sstream>
#include <memory>
#include "Loop.h"
#include "GenericComponent.h"
#include "data/FileReporter.h"
#include "data/CSVReporter.h"
#include "data/TimeReporter.h"
#include "timers/BasicTimer.h"
#include "timers/RealTimeTimer.h"
#include "timers/TimeIsUp.h"
#include "value/ValueExceed.h"

namespace simcore {

    class BasicSimulation : public simcore::Loop {

    protected:

        // timer, time reporter, stop timer
        std::unique_ptr<BasicTimer> timer{};
        std::unique_ptr<TimeReporter> timeReporter{};
        std::unique_ptr<TimeIsUp> stopTimer{};
        std::unique_ptr<FileReporter<CSVReporter>> logger{};

        std::stringstream out{};
        std::vector<std::unique_ptr<ValueExceed<double>>> stopConditions{};

        std::unique_ptr<GenericComponent> genericComponent{};

    public:


        BasicSimulation() = default;

        ~BasicSimulation() override = default;


        /**
         * Creates a simulation
         * @param endTime End time of the simulation
         * @param stepSize Time step size of the simulation
         * @param realTime Real-time flag
         * @param stopValues Values to be checked for excess (then simulation is stopped)
         */
        void create(double endTime, double stepSize, bool realTime = false,
                    const std::vector<std::pair<double *, double>> &stopValues = {}) {

            // create timer
            timer.reset(realTime ? new RealTimeTimer : new BasicTimer);

            // set timer
            timer->setTimeStepSize(stepSize);
            setTimer(timer.get());

            // stop condition (time)
            stopTimer = std::make_unique<TimeIsUp>();
            stopTimer->setStopTime(endTime);

            // stop condition (distance) TODO: test
            for (auto e : stopValues) {

                // save
                stopConditions.push_back(std::make_unique<ValueExceed<double>>());

                // get stop condition and set limits
                auto b = stopConditions.back().get();
                b->setValueAndLimit(e.first, e.second);

                // add stop condition
                addComponent(b);
                addStopCondition(b);

            }

            // add component and stop condition
            addComponent(stopTimer.get());
            addStopCondition(stopTimer.get());

            // only when real time, set time reporter
            if (realTime) {

                // set time reporter
                timeReporter = std::make_unique<TimeReporter>();
                timeReporter->setTimeStepSize(1.0);
                timeReporter->ostream(out);

                // add component
                addComponent(timeReporter.get());

            }


        }


        /**
         * Enables a file logger component
         * @param filename File name of the log file
         * @param timeStepSize Time step size
         * @return Logging component
         */
        FileReporter<CSVReporter> *enableLogging(const std::string &filename, double timeStepSize = 0.0) {

            // create logger
            logger = std::make_unique<FileReporter<CSVReporter>>();

            // setup
            logger->setFilename(filename);
            logger->setTimeStepSize(timeStepSize);

            // add logger
            addComponent(logger.get());

            // return
            return logger.get();

        }


        /**
         * Runs the simulation until end time with the given time step size
         * @param endTime End time
         * @param stepSize Time step size
         */
        void run(double endTime, double stepSize) {

            // create simulation
            create(endTime, stepSize);

            // run simulation
            Loop::run();

        }


        /**
         * Creates and returns a generic component
         * @return Generic component
         */
        GenericComponent *createGenericComponent() {

            // create generic component
            genericComponent = std::make_unique<GenericComponent>();

            // add component
            addComponent(genericComponent.get());

            // return component
            return genericComponent.get();

        }


    };

}

#endif // SIMCORE_BASIC_SIMULATION_H
