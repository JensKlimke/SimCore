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

#include <map>
#include <string>
#include <cmath>
#include <iostream>
#include "../IComponent.h"
#include "../exceptions.h"


class JsonReporter : public sim::IComponent {

    std::ostream *_outstream = nullptr;
    std::map<std::string, const double*> _values{};

    bool _hasContent = false;


public:

    /**
     * Constructor
     */
    JsonReporter() = default;


    /**
     * Adds an double value to be added to the
     * @param val Pointer to the value
     * @param key Key to be used in json
     */
    void addValue(const std::string &key, const double *val) {

        if(key == "time")
            throw std::invalid_argument("time key word is reserved.");

        _values[key] = val;

    }


    /**
     * Sets the stream in which the data shall be written
     * @param os Outstream
     */
    void setOutstream(std::ostream &os) {

        _outstream = &os;

    }


protected:


    bool step(double simTime) override {

        // save time and open object brackets
        (*_outstream) << (_hasContent ? ",\n" : "[\n") << "\t" << R"({"time":)" << simTime << ",";

        // write data
        unsigned int i = 0;
        for(auto &p : _values) {

            // stream field name
            (*_outstream) << (i++ == 0 ? "" : ",") << "\"" << p.first << "\":";

            // check for inf and nan
            if(std::isinf(*p.second) || std::isnan(*p.second))
                (*_outstream) << "null";
            else
                (*_outstream) << *p.second;

        }

        // close object brackets
        (*_outstream) << "}";

        // save that data was already written
        _hasContent = true;

        // success
        return true;

    }


    void initialize(double initTime) override {

        if(_outstream == nullptr)
            throw std::runtime_error("Output stream is not initialized.");

        _hasContent = false;

    }


    void terminate(double simTime) override {

        // close brackets
        if(_hasContent)
            (*_outstream) << "\n]" << std::endl;

    }


};


#endif //SIMCORE_JSONREPORTER_H
