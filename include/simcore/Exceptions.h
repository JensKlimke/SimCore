//
// Copyright (c) 2019 Jens Klimke <jens.klimke@rwth-aachen.de>. All rights reserved.
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
// Created by Jens Klimke on 2019-03-14
//

#ifndef SIMCORE_EXCEPTIONS_H
#define SIMCORE_EXCEPTIONS_H

#include <utility>
#include <exception>
#include <string>


class ProcessException : public std::exception
{

    std::string _msg;

public:


    explicit ProcessException(const char* msg) : _msg(msg) {}

    ProcessException(const ProcessException& ex) noexcept : _msg(ex._msg) {}

    ProcessException& operator=(const ProcessException& ex) noexcept {
        _msg = ex._msg;
        return *this;
    }

    ~ProcessException() noexcept override = default;

    const char* what() const noexcept override {
        return _msg.c_str();
    }
};


class ModelNotInitialized : public ProcessException
{

public:

    explicit ModelNotInitialized(const char* msg)
        : ProcessException(msg) {}

};


#endif //SIMCORE_SIMULATIONERRORS_H
