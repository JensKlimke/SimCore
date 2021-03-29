// Copyright (c) 2021 Jens Klimke.
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
// Created by Jens Klimke on 2021-03-27.
// Contributors:
//


#ifndef SIMCORE_DATAREPORTER_H
#define SIMCORE_DATAREPORTER_H

#include <map>
#include <string>
#include <iostream>
#include "../IComponent.h"
#include "../exceptions.h"

class DataReporter : public simcore::IComponent {

protected:

    std::ostream *_outstream = nullptr;
    std::map<std::string, const double*> _values{};


public:

    /**
     * Constructor
     */
    DataReporter() = default;


    /**
     * Adds an double value to be added to the
     * @param val Pointer to the value
     * @param key Key to be used in json
     */
    void addValue(const std::string &key, const double *val) {

        _values[key] = val;

    }


    /**
     * Sets the stream in which the data shall be written
     * @param os Output stream
     */
    void setOutstream(std::ostream &os) {

        _outstream = &os;

    }


    /**
     * Initializes the reporter
     * @param initTime Init time
     */
    void initialize(double initTime) override {

        if(_outstream == nullptr)
            throw std::runtime_error("Output stream is not initialized.");

    }


    /**
     * Terminates the reporter
     * @param simTime Termination time
     */
    void terminate(double simTime) override {

        // flush
        _outstream->flush();

    }


};

#endif //SIMCORE_DATAREPORTER_H
