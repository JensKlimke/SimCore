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
// Created by Jens Klimke on 2020-10-04.
// Contributors:
//

#include <simcore/storage/Manager.h>

#ifndef JW
#define JW(var) j[#var] = element._##var;
#endif

namespace sim::storage {

    unsigned long Manager::_counter = 0;
    std::map<const void *, unsigned long> Manager::_ids = std::map<const void *, unsigned long>{};

    unsigned long Manager::id(const void *inst) {

        // add instance if not already set
        if (_ids.find(inst) == _ids.end())
            _ids[inst] = ++Manager::_counter;

        // return ID
        return _ids[inst];

    }

    void Manager::saveLoop(sim::protobuf::Loop &p, const sim::Loop *l) {

        // set ID
        p.set_id(1);

        // stop flag
        p.set_stop(l->_stopFlag);

        // status
        switch (l->_status) {
            case Loop::Status::INITIALIZED:
                p.set_status(sim::protobuf::Loop_Status_INITIALIZED);
            case Loop::Status::RUNNING:
                p.set_status(sim::protobuf::Loop_Status_RUNNING);
            case Loop::Status::STOPPED:
                p.set_status(sim::protobuf::Loop_Status_STOPPED);
        }
//
//        // create IDs for components
//        unsigned long cid = 0;
//        for (const auto &c : l->_components)
//            componentIds.emplace(&c, cid);


    }


    void Manager::writeSetup(nlohmann::json &j, const sim::Loop &element) {

        // add arrays
        j["components"] = {};
        j["stopConditions"] = {};

        // iterate over components
        for (const auto &e : element._components)
            j["components"].emplace_back(Manager::id(e));

        // iterate over stop conditions
        for (const auto &e : element._stopConditions)
            j["stopConditions"].emplace_back(Manager::id(e));

    }


    void Manager::readSetup(const nlohmann::json &j, sim::Loop &element) {

        // TODO:

    }


    void Manager::writeState(nlohmann::json &j, const sim::Loop &element) {
        JW(status)
        JW(stopFlag)
    }


    void Manager::readState(const nlohmann::json &j, sim::Loop &element) {
        j.get_to(element._status);
        j.get_to(element._stopFlag);
    }


    void Manager::writeSetup(nlohmann::json &j, const sim::BasicTimer &element) {}

    void Manager::readSetup(const nlohmann::json &j, sim::BasicTimer &element) {}


    void Manager::writeState(nlohmann::json &j, const sim::BasicTimer &element) {
        JW(startTime)
        JW(time)
        JW(stepSize)
    }

    void Manager::readState(const nlohmann::json &j, sim::BasicTimer &element) {
        j.get_to(element._startTime);
        j.get_to(element._time);
        j.get_to(element._stepSize);
    }


    void Manager::writeSetup(nlohmann::json &j, const sim::RealTimeTimer &element) {}

    void Manager::readSetup(const nlohmann::json &j, sim::RealTimeTimer &element) {}


    void Manager::writeState(nlohmann::json &j, const sim::RealTimeTimer &element) {
        writeState(j, *dynamic_cast<const BasicTimer *>(&element));
        JW(acceleration)
        JW(startRefTime)
        JW(refTime)
        JW(steps)
    }

    void Manager::readState(const nlohmann::json &j, sim::RealTimeTimer &element) {
        readState(j, *dynamic_cast<BasicTimer *>(&element));
        j.get_to(element._acceleration);
        j.get_to(element._startRefTime);
        j.get_to(element._refTime);
        j.get_to(element._steps);
    }


    void Manager::writeSetup(nlohmann::json &j, const sim::TimeIsUp &element) {}

    void Manager::readSetup(const nlohmann::json &j, sim::TimeIsUp &element) {}

    void Manager::writeState(nlohmann::json &j, const sim::TimeIsUp &element) {
        JW(stopTime)
        JW(code)
    }

    void Manager::readState(const nlohmann::json &j, sim::TimeIsUp &element) {

    }


}