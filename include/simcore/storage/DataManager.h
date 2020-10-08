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


#ifndef SIMCORE_DATA_MANAGER_H
#define SIMCORE_DATA_MANAGER_H

#include <map>
#include <string>
#include "SignalInterface.h"

namespace sim {

    struct DataManager {

        struct Entry {
            std::string name{};
            void *instance = nullptr;
            std::map<std::string, sim::storage::SignalInterface *> signals{};
        };

        static std::map<const void *, Entry> index;

        static void registerSignal(const void *owner, const std::string &name, sim::storage::SignalInterface *signal);

        static sim::storage::SignalInterface *getSignal(const void *owner, const std::string &name);

        static void registerOwner(void *owner, const std::string &name);

        static const std::string &getOwner(const void *owner);

    };

}

#endif // SIMCORE_DATA_MANAGER_H
