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


#ifndef SIMCORE_MANAGER_H
#define SIMCORE_MANAGER_H

#include <map>
#include <nlohmann/json.hpp>
#include <SimCore.pb.h>
#include <simcore/Loop.h>
#include <simcore/timers/BasicTimer.h>
#include <simcore/timers/RealTimeTimer.h>
#include <simcore/timers/TimeIsUp.h>


namespace sim::storage {

    class Manager {

        static unsigned long _counter;
        static std::map<const void *, unsigned long> _ids;

    public:

        /**
         * @brief Returns the ID of the instance
         * Creates the ID of the instance if not existing
         * @return ID of the instance
         */
        static unsigned long id(const void *);

        static void saveLoop(sim::protobuf::Loop &p, const sim::Loop *l);

        static void writeSetup(nlohmann::json &j, const sim::Loop &loop);

        static void readSetup(const nlohmann::json &j, sim::Loop &loop);

        static void writeState(nlohmann::json &j, const sim::Loop &loop);

        static void readState(const nlohmann::json &j, sim::Loop &loop);


        static void writeSetup(nlohmann::json &j, const sim::BasicTimer &element);

        static void readSetup(const nlohmann::json &j, sim::BasicTimer &element);

        static void writeState(nlohmann::json &j, const sim::BasicTimer &element);

        static void readState(const nlohmann::json &j, sim::BasicTimer &element);


        static void writeSetup(nlohmann::json &j, const sim::RealTimeTimer &element);

        static void readSetup(const nlohmann::json &j, sim::RealTimeTimer &element);

        static void writeState(nlohmann::json &j, const sim::RealTimeTimer &element);

        static void readState(const nlohmann::json &j, sim::RealTimeTimer &element);


        static void writeSetup(nlohmann::json &j, const sim::TimeIsUp &element);

        static void readSetup(const nlohmann::json &j, sim::TimeIsUp &element);

        static void writeState(nlohmann::json &j, const sim::TimeIsUp &element);

        static void readState(const nlohmann::json &j, sim::TimeIsUp &element);


    };

}


template<class T>
void to_json(nlohmann::json &j, const T &element) {
    j = nlohmann::json{};
    sim::storage::Manager::writeSetup(j, element);
    sim::storage::Manager::writeState(j, element);
}


template<class T>
void from_json(const nlohmann::json &j, T &element) {
    sim::storage::Manager::readSetup(j, element);
    sim::storage::Manager::readState(j, element);
}


#endif //SIMCORE_MANAGER_H
