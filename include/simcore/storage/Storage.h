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
// Created by Jens Klimke on 2020-10-02.
//


#ifndef SIMCORE_STORAGE_H
#define SIMCORE_STORAGE_H

#include <string>
#include <map>

namespace sim::storage {

    struct DataSignal {

        std::string context{};
        bool isNode = false;
        void *pointer = nullptr;

    };

    struct DataNode {

        // container for signals
        std::map<std::string, DataSignal> signals{};

        template<typename T>
        T *get(const std::string &key) {
            return (T *) signals.at(key).pointer;
        }

        template<typename T>
        const T *get_const(const std::string &key) const {
            return (const T *) signals.at(key).pointer;
        }

        template<typename T>
        void write(T &target, const std::string &key) const {
            target = *get_const<T>(key);
        }

        void create(const std::string &key, void *ptr) {
            signals.emplace(key, DataSignal{"", false, ptr});
        }

    };

}

#define REGISTER_SIGNAL(_name) \
node.signals.emplace(#_name, &(_name))

#define REGISTER_SIGNAL_ALIAS(_name, _alias) \
node.create(#_alias, (void*) &(_name))

#define REGISTER_SIGNAL_CONTEXT(_name, _context) \
node.create(#_name, (void*) &(_context._name))

#define REGISTER_SIGNAL_CONTEXT_ALIAS(_name, _context, _alias) \
node.create(#_alias, (void*) &(_context._name))

#define WRITE_ALIAS(_name, _alias) \
node.write((_name), #_alias);

#define WRITE_CONTEXT(_name, _context) \
node.write((_context._name), #_name);

#define WRITE_CONTEXT_ALIAS(_name, _context, _alias) \
node.write((_context._name), #_alias);

#endif // SIMCORE_STORAGE_H
