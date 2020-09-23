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
// Created by Jens Klimke on 2020-09-23.
// Contributors:
//


#include <simcore/framework/ModelManager.h>
#include <simcore/timers/BasicTimer.h>
#include <simcore/timers/TimeIsUp.h>

namespace sim::framework {

    sim::ITimer *create_basic_timer(const Config &c) {

        auto timer = new sim::BasicTimer;

        if(c["startTime"].IsDefined())
            timer->setStartTime(c["startTime"].as<double>());

        if(c["stepSize"])
            timer->setTimeStepSize(c["stepSize"].as<double>());

        return timer;

    }

    sim::IComponent *create_time_is_up(const Config &c) {

        // create object
        auto timer = new sim::TimeIsUp;

        // stop time
        if(c["stopTime"].IsDefined())
            timer->setStopTime(c["stopTime"].as<double>());

        // return instance
        return timer;

    }

    std::map<std::string, CreatorFunction> ModelManager::_index{
            {"TimeIsUp", create_time_is_up}
    };


    ModelManager::ModelManager() = default;


    ModelManager::~ModelManager() {

        // delete creations
        for(auto &m : _creations)
            delete m;

    }


    void ModelManager::registerModelType(const std::string &type, CreatorFunction &&factory) {

        // register creator function
        _index.emplace(type, std::move(factory));

    }


    sim::IComponent *ModelManager::create(const std::string &type, const Config &parameters) {

        // create and store object
        auto obj = _index[type](parameters);
        _creations.emplace_back(obj);

        // return object
        return obj;

    }

}