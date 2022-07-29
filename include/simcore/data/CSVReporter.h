//
// Copyright (c) 2019-2020 Jens Klimke <jens.klimke@rwth-aachen.de>
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
// Created by Jens Klimke on 2019-03-16
//

#ifndef SIMCORE_JSONREPORTER_H
#define SIMCORE_JSONREPORTER_H

#include <cmath>
#include "StreamReporter.h"

class CSVReporter : public StreamReporter {

public:

    CSVReporter() = default;
    ~CSVReporter() override = default;


protected:


    void step(double t, double dt) override {

        // save time and open object brackets
        (*_outstream) << std::endl << t << "," << dt;

        // write data
        for(auto &p : _values) {

            // check for inf and nan
            if(std::isinf(*p.second) || std::isnan(*p.second))
                (*_outstream) << ",";
            else
                (*_outstream) << "," << *p.second;

        }

    }


    void initialize(double initTime) override {

        StreamReporter::initialize(initTime);

        // write header
        (*_outstream) << "time,timeStepSize";

        // iterate over fields
        for(auto &p : _values) {

            // check for inf and nan
            (*_outstream) << "," << p.first;

        }

    }


};


#endif // SIMCORE_JSONREPORTER_H
