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
// Created by Jens Klimke on 2019-04-01
//

#ifndef SIMCORE_VALUEOUTOFTUBE_H
#define SIMCORE_VALUEOUTOFTUBE_H

#include "../IStopCondition.h"
#include "../IComponent.h"
#include "../exceptions.h"
#include "SignalTube.h"

class ValueOutOfTube : public SignalTube, public sim::IStopCondition, public sim::IComponent {

    IStopCondition::StopCode _mode = IStopCondition::StopCode::OBJECTIVES_MISSED;

    const double *_x = nullptr;
    const double *_y = nullptr;

public:

    typedef IStopCondition::StopCode Mode;

    ValueOutOfTube() = default;

    void initialize(double initTime) override {

        if(!isSet())
            throw ModelNotInitialized("Band is not initialized.");

    }

    bool step(double simTime) override {

        if(!in(*_x, *_y))
            failed();

        return true;

    }

    void terminate(double simTime) override {

    }


    /**
     * Sets the values to be checked
     * @param x x-value
     * @param y y-value
     */
    void setValues(const double *x, const double *y, Mode mode = Mode::OBJECTIVES_MISSED) {

        _x = x;
        _y = y;
        _mode  = mode;

    }


};

#endif //SIMCORE_VALUEOUTOFTUBE_H
