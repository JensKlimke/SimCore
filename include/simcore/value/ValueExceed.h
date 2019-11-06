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
// Created by Jens Klimke on 2019-03-30
//

#ifndef SIMCORE_VALUEEXCEED_H
#define SIMCORE_VALUEEXCEED_H

#include "../IStopCondition.h"
#include "../IComponent.h"

template<typename T>
class ValueExceed : public ::sim::IStopCondition, public ::sim::IComponent {

    IStopCondition::StopCode _mode = IStopCondition::StopCode::SIM_ENDED;

    const T* _value = nullptr;
    T _limit{};

public:

    typedef IStopCondition::StopCode Mode;

    void setValueAndLimit(const T* value, const T &limit, Mode mode = Mode::SIM_ENDED) {

        _value = value;
        _limit = limit;
        _mode  = mode;

    }


    void initialize(double initTime) override {

        reset();

    }


    bool step(double simTime) override {

        // the limit has been reached
        if(*_value > _limit)
            stop(_mode);

        return true;

    }


    void terminate(double simTime) override {

    }

};


#endif //SIMCORE_VALUEEXCEED_H