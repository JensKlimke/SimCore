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
// Created by Jens Klimke on $YEAR-$MONTH-10.
//


#ifndef SIMCORE_SAMPLE_APPLICATION_H
#define SIMCORE_SAMPLE_APPLICATION_H

#include <simcore/Model.h>
#include <map>

class SampleApplication : public sim::Model {

protected:

    unsigned long i = 0;
    double initTime = 0.0;
    std::map<unsigned long, std::pair<double, double>> simTimes{};
    double termTime = 0.0;

    void initialize(double t) override {
        initTime = t;
    }

    void step(double t, double dt) override {
        simTimes[i++] = {t, dt};
    }

    void terminate(double t) override {
        termTime = t;
    }

};

#endif //SIMCORE_SAMPLE_APPLICATION_H
