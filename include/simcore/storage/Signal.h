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
// Created by Jens Klimke on 2020-08-06.
//


#ifndef SIMCORE_SIGNAL_H
#define SIMCORE_SIGNAL_H

#include <string>
#include <functional>
#include <utility>
#include "DataManager.h"

namespace sim::storage {

    template<typename T>
    struct Signal : public SignalInterface {

        T value;
        std::string name{};
        const void *owner = nullptr;

        Signal() = delete;

        Signal(const void *o, std::string &&n) : owner(o), name(n), value() {
            reg();
        }

        Signal(const void *o, std::string &&n, T &&v) : owner(o), name(n), value(v) {
            reg();
        }


        void reg() {
#ifdef MANAGE_SIGNALS
            sim::DataManager::registerSignal(owner, name, this);
#endif
        }


        /**
         * Writes the value to the JSON object
         * @param obj JSON object
         */
        void toJson(nlohmann::json &obj) const override {
            obj[name] = value;
        }


        /**
         * Reads the value from JSON object
         * @param obj JSON object
         */
        void fromJson(nlohmann::json &obj) override {
            obj[name].get_to(value);
        }


        /*!< conversion operator */
        explicit operator T() const { return value; }

        /*!< assignment operator */
        Signal &operator=(T v) {
            value = std::move(v);
            return *this;
        }

        /*!< comparison operators */
        bool operator==(const T &v) const { return value == v; }

        bool operator!=(const T &v) const { return value != v; }

        bool operator<(const T &v) const { return value < v; }

        bool operator<=(const T &v) const { return value <= v; }

        bool operator>(const T &v) const { return value > v; }

        bool operator>=(const T &v) const { return value >= v; }


        /*!< math operators */
        T operator+(const T &v) const { return value + v; }

        T operator-(const T &v) const { return value - v; }

        T operator*(const T &v) const { return value * v; }

        T operator/(const T &v) const { return value / v; }

        T operator+(const Signal<T> &v) const { return value + v.value; }

        T operator-(const Signal<T> &v) const { return value - v.value; }

        T operator*(const Signal<T> &v) const { return value * v.value; }

        T operator/(const Signal<T> &v) const { return value / v.value; }


        Signal<T> &operator++() {
            ++value;
            return *this;
        }

        Signal<T> &operator--() {
            --value;
            return *this;
        }

        Signal &operator+=(T v) {
            value += v;
            return *this;
        }

        Signal &operator-=(T v) {
            value += v;
            return *this;
        }

        Signal &operator+=(const Signal<T> &v) {
            value += v.value;
            return *this;
        }

        Signal &operator-=(const Signal<T> &v) {
            value += v.value;
            return *this;
        }

    };


} // namespace: sim::storage


namespace sim {

    typedef storage::Signal<double> Double;
    typedef storage::Signal<float> Float;
    typedef storage::Signal<bool> Bool;
    typedef storage::Signal<char> Char;
    typedef storage::Signal<unsigned char> UChar;
    typedef storage::Signal<short> Short;
    typedef storage::Signal<unsigned short> UShort;
    typedef storage::Signal<int> Int;
    typedef storage::Signal<unsigned int> UInt;
    typedef storage::Signal<long> Long;
    typedef storage::Signal<unsigned long> ULong;
    typedef storage::Signal<long long> LongLong;
    typedef storage::Signal<unsigned long long> ULongLong;
    typedef storage::Signal<std::string> String;

}

template<typename T>
bool operator==(const T &a, const sim::storage::Signal<T> b) { return a == b.value; }

template<typename T>
bool operator!=(const T &a, const sim::storage::Signal<T> b) { return a != b.value; }

template<typename T>
bool operator<(const T &a, const sim::storage::Signal<T> b) { return a < b.value; }

template<typename T>
bool operator<=(const T &a, const sim::storage::Signal<T> b) { return a <= b.value; }

template<typename T>
bool operator>(const T &a, const sim::storage::Signal<T> b) { return a > b.value; }

template<typename T>
bool operator>=(const T &a, const sim::storage::Signal<T> b) { return a >= b.value; }


template<typename T>
T operator+(const T &a, const sim::storage::Signal<T> b) { return a + b.value; }

template<typename T>
T operator-(const T &a, const sim::storage::Signal<T> b) { return a - b.value; }

template<typename T>
T operator*(const T &a, const sim::storage::Signal<T> b) { return a * b.value; }

template<typename T>
T operator/(const T &a, const sim::storage::Signal<T> b) { return a / b.value; }

#endif // SIMCORE_SIGNAL_H
