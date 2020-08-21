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
// Created by Jens Klimke on 2020-08-21.
//


#ifndef SIMCORE_PID_CONTROLLER_INTERFACE_H
#define SIMCORE_PID_CONTROLLER_INTERFACE_H

#include <simcore/blocks/Block.h>

struct Input {
    double targetVelocity;
};

struct Output {
    double velocity;
    double acceleration;
};

struct State {
    SIGNAL(block.test, target)
    SIGNAL(block.test, delay)
    SIGNAL(block.test, error)
    SIGNAL(block.test, control)
    SIGNAL(block.test, system)
};

struct MemoryDelay {
    double value;
};

struct MemoryIntegral {
    double value;
};

struct MemoryPID {
    double sum;
    double err;
};

struct Memory {
    MemoryDelay delay;
    MemoryPID pid;
    MemoryIntegral integral;
};

struct ParamPID {
    double kP;
    double kI;
    double kD;
};

struct Parameters {
    ParamPID pid;
};

#endif //SIMCORE_PID_CONTROLLER_INTERFACE_H
