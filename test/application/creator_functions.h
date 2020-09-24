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
// Created by Jens Klimke on $YEAR-$MONTH-24.
//


#ifndef SIMCORE_CREATOR_FUNCTIONS_H
#define SIMCORE_CREATOR_FUNCTIONS_H

#include <simcore/timers/BasicTimer.h>
#include <simcore/timers/RealTimeTimer.h>
#include <simcore/timers/TimeIsUp.h>
#include <simcore/logging/TimeReporter.h>
#include <simcore/value/ValueExceed.h>
#include <simcore/framework/ComponentManager.h>

namespace sim::framework {


    struct DataNode {

        void *_value;

        template<class T>
        [[nodiscard]] T *as() const {
            return reinterpret_cast<T*>(_value);
        }

    };

    struct DataManager : public std::map<std::string, std::unique_ptr<DataNode>> {};


    sim::BasicTimer *create_basic_timer(const config_t &c) {

        auto timer = new sim::BasicTimer;

        if (c["startTime"].IsDefined())
            timer->setStartTime(c["startTime"].as<double>());

        if (c["stepSize"])
            timer->setTimeStepSize(c["stepSize"].as<double>());

        return timer;

    }

    sim::RealTimeTimer *create_rt_timer(const config_t &c) {

        auto timer = new sim::RealTimeTimer;

        if (c["startTime"].IsDefined())
            timer->setStartTime(c["startTime"].as<double>());

        if (c["stepSize"])
            timer->setTimeStepSize(c["stepSize"].as<double>());

        if (c["acceleration"])
            timer->setAcceleration(c["acceleration"].as<double>());

        return timer;

    }

    sim::TimeIsUp *create_time_is_up(const config_t &c) {

        // create object
        auto timer = new sim::TimeIsUp;

        // stop time
        if (c["stopTime"].IsDefined())
            timer->setStopTime(c["stopTime"].as<double>());

        // return instance
        return timer;

    }


    sim::logging::TimeReporter *create_time_reporter(const config_t &c) {

        // create object
        auto timer = new sim::logging::TimeReporter;

        // time step size
        if (c["timeStepSize"].IsDefined())
            timer->setTimeStepSize(c["timeStepSize"].as<double>());

        //
        if (c["outputStream"].IsDefined())
            if(c["outputStream"].as<std::string>() == "console")
                timer->ostream(std::cout);

        // return instance
        return timer;

    }


    template<typename T>
    sim::logging::TimeReporter *create_value_exceed(const config_t &c, DataManager &data) {

        // create object
        auto ve = new sim::value::ValueExceed<T>;

        // get value
        auto key = c["key"].as<std::string>();
        auto limit = c["limit"].as<T>();

        auto result = sim::IStopCondition::StopCode::NONE;
        if(c["result"].IsDefined()) {

            // result switch
            if(c["result"].as<std::string>() == "FAILED")
                result = sim::IStopCondition::StopCode::OBJECTIVES_MISSED;
            else if(c["result"].as<std::string>() == "SUCCEED")
                result = sim::IStopCondition::StopCode::OBJECTIVES_REACHED;
            else if(c["result"].as<std::string>() == "END")
                result = sim::IStopCondition::StopCode::SIM_ENDED;

        }

        // get value
        auto value = data[key]->as<double>();

        // set value, limit and result
        ve->setValueAndLimit(value, limit, result);

        // return instance
        return ve;

    }


}

#endif // SIMCORE_CREATOR_FUNCTIONS_H
