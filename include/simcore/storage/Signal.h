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
#include "SignalInterface.h"

#define UPDATE(N) {     \
    auto old = _value;  \
    N;                  \
    update(old);        \
}

namespace sim::storage {

    template<typename T>
    class Signal : public SignalInterface {

    protected:

        T _value{};           //!< The value of the signal
        const void *_owner{}; //!< The pointer to the owner
        std::string _name{};  //!< The name of the signal

        std::function<void(const T &newValue, const T &oldValue, const Signal<T> &signal)> _onUpdate
                = nullptr; //!< The callback which is called when the value is updated


    public:


        /**
         * Basic constructor
         */
        Signal() = delete;


        /**
         * Constructor
         * @param owner Owner of the signal
         * @param name Signal name
         */
        Signal(const void *owner, std::string &&name)
                : _owner{owner}, _name{std::move(name)} {}


        /**
         * Constructor
         * @param owner Owner of the signal
         * @param name Signal name
         * @param def Default value
         */
        Signal(const void *owner, std::string &&name, T &&def)
                : _owner{owner}, _name{std::move(name)}, _value(std::move(def)) {}


        /**
         * Returns the name of the signal
         * @return
         */
        [[nodiscard]] const std::string &getName() const {

            // return the name
            return _name;

        }


        /**
         * Returns the pointer to the owner
         * @return Owner pointer
         */
        [[nodiscard]] const void *getOwner() const {

            return _owner;

        }


        /**
         * Sets the callback to be executed when the signal is updated
         * @param callback Callback function to be set
         */
        void
        setUpdateCallback(std::function<void(const T &newValue, const T &oldValue, const Signal &signal)> callback) {

            // set function
            _onUpdate = callback;

        }


        /**
         * Assignment operator
         * @param value Value to be set
         * @return Returns the signal object
         */
        Signal &operator=(T value) {

            // execute update and return this object
            UPDATE(_value = value)
            return *this;

        }


        explicit operator T() const { return _value; }

        explicit operator T() { return _value; }

        bool operator==(const T &b) const { return _value == b; }

        bool operator!=(const T &b) const { return _value != b; }

        bool operator<(const T &b) const { return _value < b; }

        bool operator>(const T &b) const { return _value > b; }

        bool operator<=(const T &b) const { return _value <= b; }

        bool operator>=(const T &b) const { return _value >= b; }

        T operator+() const { return +_value; }

        T operator-() const { return +_value; }

        T operator~() const { return ~_value; }

        T operator+(const Signal<T> &b) const { return _value + b._value; }

        T operator-(const Signal<T> &b) const { return _value - b._value; }

        T operator*(const Signal<T> &b) const { return _value * b._value; }

        T operator/(const Signal<T> &b) const { return _value / b._value; }

        T operator%(const Signal<T> &b) const { return _value % b._value; }

        T operator&(const Signal<T> &b) const { return _value & b._value; }

        T operator|(const Signal<T> &b) const { return _value | b._value; }

        T operator^(const Signal<T> &b) const { return _value ^ b._value; }

        T operator<<(const Signal<T> &b) const { return _value << b._value; }

        T operator>>(const Signal<T> &b) const { return _value >> b._value; }

        T &operator+=(const Signal<T> &b) {
            UPDATE(_value += b._value)
            return _value;
        }

        T &operator-=(const Signal<T> &b) {
            UPDATE(_value -= b._value)
            return _value;
        }

        T &operator*=(const Signal<T> &b) {
            UPDATE(_value *= b._value)
            return _value;
        }

        T &operator/=(const Signal<T> &b) {
            UPDATE(_value /= b._value)
            return _value;
        }

        T &operator%=(const Signal<T> &b) {
            UPDATE(_value %= b._value)
            return _value;
        }

        T &operator&=(const Signal<T> &b) {
            UPDATE(_value &= b._value)
            return _value;
        }

        T &operator|=(const Signal<T> &b) {
            UPDATE(_value |= b._value)
            return _value;
        }

        T &operator^=(const Signal<T> &b) {
            UPDATE(_value ^= b._value)
            return _value;
        }

        T &operator<<=(const Signal<T> &b) {
            UPDATE(_value <<= b._value)
            return _value;
        }

        T &operator>>=(const Signal<T> &b) {
            UPDATE(_value >>= b._value)
            return _value;
        }

        T operator++() { return ++_value; }

        T operator--() { return --_value; }

        T operator+(const T &b) const { return _value + b; }

        T operator-(const T &b) const { return _value - b; }

        T operator*(const T &b) const { return _value * b; }

        T operator/(const T &b) const { return _value / b; }

        T operator%(const T &b) const { return _value % b; }

        T operator&(const T &b) const { return _value & b; }

        T operator|(const T &b) const { return _value | b; }

        T operator^(const T &b) const { return _value ^ b; }

        T operator<<(const T &b) const { return _value << b; }

        T operator>>(const T &b) const { return _value >> b; }

        T &operator+=(const T &b) {
            UPDATE(_value += b)
            return _value;
        }

        T &operator-=(const T &b) {
            UPDATE(_value -= b)
            return _value;
        }

        T &operator*=(const T &b) {
            UPDATE(_value *= b)
            return _value;
        }

        T &operator/=(const T &b) {
            UPDATE(_value /= b)
            return _value;
        }

        T &operator%=(const T &b) {
            UPDATE(_value %= b)
            return _value;
        }

        T &operator&=(const T &b) {
            UPDATE(_value &= b)
            return _value;
        }

        T &operator|=(const T &b) {
            UPDATE(_value |= b)
            return _value;
        }

        T &operator^=(const T &b) {
            UPDATE(_value ^= b)
            return _value;
        }

        T &operator<<=(const T &b) {
            UPDATE(_value <<= b)
            return _value;
        }

        T &operator>>=(const T &b) {
            UPDATE(_value >>= b)
            return _value;
        }


    protected:


        /**
         * Method to be called, when value has been changed
         * @param old The old value
         */
        void update(const T &old) {

            // callback
            if (_onUpdate)
                _onUpdate(_value, old, *this);

        }

    };


//    template<typename T>
//    const Signal<T> *signal(const void *owner, const std::string &key) {
//
//        return (sim::storage::Signal<T> *) (DataManager::getSignal(owner, key));
//
//    }
//
//
//    template<typename T>
//    T value(const void *owner, const std::string &key) {
//
//        return (T) *signal<T>(owner, key);
//
//    }


} // namspace: sim::storage


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

#endif // SIMCORE_SIGNAL_H
