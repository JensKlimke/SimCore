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
// Created by Jens Klimke on $YEAR-$MONTH-06.
//


#ifndef SIMCORE_SIGNAL_H
#define SIMCORE_SIGNAL_H


template<typename T>
class Signal {

public:

    std::string _name;
    T _value{};

    explicit Signal(std::string &&owner, std::string &&name) : _name(owner + "." + name) {}

    Signal &operator= (T value) {
        update(_value = value);
        return *this;
    }

    operator T() const { return _value; }

    T operator+() const { return +_value; }
    T operator-() const { return +_value; }
    T operator~() const  { return ~_value; }

    T operator+(const Signal<T> &b) const { return _value + b._value; }
    T operator-(const Signal<T> &b) const { return _value - b._value; }
    T operator*(const Signal<T> &b) const { return _value * b._value; }
    T operator/(const Signal<T> &b) const { return _value / b._value; }
    T operator%(const Signal<T> &b) const  { return _value % b._value; }
    T operator&(const Signal<T> &b) const  { return _value & b._value; }
    T operator|(const Signal<T> &b) const  { return _value | b._value; }
    T operator^(const Signal<T> &b) const  { return _value ^ b._value; }
    T operator<<(const Signal<T> &b) const  { return _value << b._value; }
    T operator>>(const Signal<T> &b) const  { return _value >> b._value; }

    T &operator+=(const Signal<T> &b) { return update(_value += b._value); }
    T &operator-=(const Signal<T> &b) { return update(_value -= b._value); }
    T &operator*=(const Signal<T> &b) { return update(_value *= b._value); }
    T &operator/=(const Signal<T> &b) { return update(_value /= b._value); }
    T &operator%=(const Signal<T> &b) { return update(_value %= b._value); }
    T &operator&=(const Signal<T> &b) { return update(_value &= b._value); }
    T &operator|=(const Signal<T> &b) { return update(_value |= b._value); }
    T &operator^=(const Signal<T> &b) { return update(_value ^= b._value); }
    T &operator<<=(const Signal<T> &b) { return update(_value <<= b._value); }
    T &operator>>=(const Signal<T> &b) { return update(_value >>= b._value); }

    T operator+(const T &b) const { return _value + b; }
    T operator-(const T &b) const { return _value - b; }
    T operator*(const T &b) const { return _value * b; }
    T operator/(const T &b) const { return _value / b; }
    T operator%(const T &b) const  { return _value % b; }
    T operator&(const T &b) const  { return _value & b; }
    T operator|(const T &b) const  { return _value | b; }
    T operator^(const T &b) const  { return _value ^ b; }
    T operator<<(const T &b) const  { return _value << b; }
    T operator>>(const T &b) const  { return _value >> b; }

    T &operator+=(const T &b) { return update(_value += b); }
    T &operator-=(const T &b) { return update(_value -= b); }
    T &operator*=(const T &b) { return update(_value *= b); }
    T &operator/=(const T &b) { return update(_value /= b); }
    T &operator%=(const T &b) { return update(_value %= b); }
    T &operator&=(const T &b) { return update(_value &= b); }
    T &operator|=(const T &b) { return update(_value |= b); }
    T &operator^=(const T &b) { return update(_value ^= b); }
    T &operator<<=(const T &b) { return update(_value <<= b); }
    T &operator>>=(const T &b) { return update(_value >>= b); }

    T &update(const T &value) {

        // TODO: do something
        // std::cout << "Value \"" << _name << "\" has been changed to " << _value << std::endl << std::flush;

        // return value
        return _value;

    }

};

#endif // SIMCORE_SIGNAL_H
