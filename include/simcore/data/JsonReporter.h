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


class JsonReporter : public DataReporter {

public:

    /**
     * Constructor
     */
    JsonReporter() = default;


protected:


    void step(double t, double dt) override {

        // save time and open object brackets
        (*_outstream) << "\t" << R"({"time":)" << t << "," << R"("timeStepSize":)" << dt << ",";

        // write data
        for(auto &p : _values) {

            // stream field name
            (*_outstream) << ",\"" << p.first << "\":";

            // check for inf and nan
            if(std::isinf(*p.second) || std::isnan(*p.second))
                (*_outstream) << "null";
            else
                (*_outstream) << *p.second;

        }

        // close object brackets
        (*_outstream) << "}";

    }


    void initialize(double initTime) override {

        // init data reporter
        DataReporter::initialize(initTime);

        (*_outstream) << "[" << std::endl;

    }


    void terminate(double simTime) override {

        // close brackets
        (*_outstream) << "]" << std::endl;

        // terminate data reporter
        DataReporter::terminate(simTime);

    }


};


#endif //SIMCORE_JSONREPORTER_H
