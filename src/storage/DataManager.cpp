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

namespace sim::storage {

    std::map<const void *, DataManager::Entry> DataManager::_entries = std::map<const void *, DataManager::Entry>{};

    void DataManager::addSignal(const void *owner, const std::string &key, SignalInterface *signal) {

        // add to map, if instance not registered
        if (_entries.find(owner) == _entries.end())
            _entries.emplace(owner, Entry{});

        // add signal
        _entries[owner].signals.emplace(key, signal);

    }


    SignalInterface *DataManager::getSignal(const void *owner, const std::string &key) {

        // return signal interface
        return _entries.at(owner).signals.at(key);

    }


}
