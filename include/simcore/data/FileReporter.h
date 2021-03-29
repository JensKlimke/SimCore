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
// Created by Jens Klimke.

#ifndef SIMCORE_FILEREPORTER_H
#define SIMCORE_FILEREPORTER_H

#include <fstream>
#include <memory>


template<class T>
class FileReporter : public T {

protected:

    std::fstream _fstream;
    std::string _filename;


public:

    FileReporter() = default;
    ~FileReporter() override = default;


    void setFilename(const std::string &name) {

        // set filename
        _filename = name;

    }


protected:


    /**
     * Initialize the reporter
     * @param initTime Initialization time
     */
    void initialize(double initTime) override {

        // open file and set as output stream
        _fstream.open(_filename, std::ios::out);
        T::setOutstream(_fstream);

        // initialize
        T::initialize(initTime);

    }


    /**
     * Terminates the reporter
     * @param termTime Termination time
     */
    void terminate(double termTime) override {

        // terminate
        T::terminate(termTime);

        // close out stream
        _fstream.close();

    }


};


#endif //SIMCORE_FILEREPORTER_H
