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
// Created by Jens Klimke on 2020-09-25.
//


#ifndef SIMCORE_SIM_FUNCTIONS_H
#define SIMCORE_SIM_FUNCTIONS_H

#include "types.h"

#if _WIN32
#include <windows.h>
#endif

namespace sim {


    /**
     * Bounds the given value to the maximum and minimum
     * @tparam T Value type
     * @param value Value to be bounded
     * @param minValue Lower bound
     * @param maxValue Upper bound
     * @return
     */
    template<typename T>
    inline T range(T value, T minValue, T maxValue) {
        using namespace std;
        return max(min(value, maxValue), minValue);
    }


    /**
     * String formatter
     * @tparam Args
     * @param format Format string
     * @param args Inputs
     * @return Formatted string
     */
    template<typename ... Args>
    std::string string_format(const std::string &format, Args ... args) {
        auto size = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
        char *buf(new char[size]);
        snprintf(buf, size, format.c_str(), args ...);
        return std::string(buf, buf + size - 1); // We don't want the '\0' inside
    }


}

#endif //SIMCORE_SIM_FUNCTIONS_H
