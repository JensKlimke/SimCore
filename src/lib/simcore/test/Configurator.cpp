// Copyright (c) 2020 Jens Klimke.
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
// Created by Jens Klimke on $date.get('yyyy-M-d').
// Contributors:
//


#include <simcore/test/Configurator.h>
#include <nlohmann/json.hpp>
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <iostream>
#include <simcore/timers/BasicTimer.h>
#include <simcore/timers/RealTimeTimer.h>
#include <simcore/timers/TimeIsUp.h>
#include <simcore/data/TimeReporter.h>

namespace sim::test {


    Configurator::Configurator(const std::string &filename) {

        YAML::Node config = YAML::LoadFile(filename);

        // set timer
        if(config["realtime"].IsDefined()) {

            // create timer
            auto t = new RealTimeTimer();

            // get timer from yaml
            auto timer = config["realtime"];

            // configuration
            t->setAcceleration(timer["acceleration"].as<double>());
            t->setTimeStepSize(timer["timeStepSize"].as<double>());
            t->setStartTime(timer["startTime"].as<double>());

            // save timer
            this->setTimer(t);

        } else if(config["time"].IsDefined()) {

            // create timer
            auto t = new BasicTimer();

            // get timer from yaml
            auto timer = config["time"];

            // configuration
            t->setTimeStepSize(timer["timeStepSize"].as<double>());
            t->setStartTime(timer["startTime"].as<double>());

            // save timer
            this->setTimer(t);

        }

        // stop conditions
        if(config["stops"].IsDefined()) {

            // iterate over stop conditions
            for(auto const &sc : config["stops"]) {

                if(sc["time"].IsDefined()) {

                    // create stop time
                    auto stopTime = new TimeIsUp();

                    // set parameters
                    stopTime->setStopTime(sc["time"]["stopTime"].as<double>());

                    // set stop condition and component
                    this->addStopCondition(stopTime);
                    this->addComponent(stopTime);

                }

            }

        }


        // components
        if(config["reporters"].IsDefined()) {

            // iterate over stop conditions
            for(auto const &rep : config["reporters"]) {

                if(rep["time"].IsDefined()) {

                    // create stop time
                    auto timeReporter = new data::TimeReporter();

                    // set parameters
                    timeReporter->setStartTime(rep["time"]["startTime"].as<double>());
                    timeReporter->setTimeStepSize(rep["time"]["interval"].as<double>());

                    // add component
                    this->addComponent(timeReporter);

                }

            }

        }

    }


    Configurator::~Configurator() {

        delete _timer;

    }


}