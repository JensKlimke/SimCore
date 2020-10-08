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
// Created by Jens Klimke on $YEAR-$MONTH-05.
//

#include <simcore/storage/DataManager.h>

namespace sim {

    std::map<const void *, DataManager::Entry> DataManager::index = std::map<const void *, DataManager::Entry>{};

    void
    DataManager::registerSignal(const void *owner, const std::string &name, sim::storage::SignalInterface *signal) {

        // add to map, if instance not registered
        if (index.find(owner) == index.end())
            index.emplace(std::pair<const void *, Entry>(owner, {}));

        // add signal
        index[owner].signals.emplace(name, signal);

    }


    sim::storage::SignalInterface *DataManager::getSignal(const void *owner, const std::string &key) {

        // return signal interface
        return index.at(owner).signals.at(key);

    }


    void DataManager::registerOwner(void *owner, const std::string &name) {


        if (index.find(owner) == index.end()) {

            // create entry
            index.emplace(std::pair<const void *, Entry>(owner, Entry{name, owner, {}}));

        } else {

            // set instance and name
            index.at(owner).instance = owner;
            index.at(owner).name = name;

        }

    }


    const std::string &DataManager::getOwner(const void *owner) {

        return index.at(owner).name;

    }


}
