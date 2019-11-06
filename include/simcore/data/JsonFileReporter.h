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
// Created by Jens Klimke.

#ifndef SIMCORE_JSONFILEREPORTER_H
#define SIMCORE_JSONFILEREPORTER_H

#include "JsonReporter.h"
#include <fstream>


class JsonFileReporter : public JsonReporter {


    std::fstream _fstream;
    std::string _filename;

public:

    JsonFileReporter() = default;

    ~JsonFileReporter() override = default;


    void setFilename(const std::string &name) {

        _filename = name;

    }


protected:



    void initialize(double initTime) override {

        _fstream.open(_filename, std::ios::out);
        setOutstream(_fstream);

        JsonReporter::initialize(initTime);

    }


    void terminate(double simTime) override {

        JsonReporter::terminate(simTime);

        _fstream.close();

    }


};


#endif //SIMCORE_JSONFILEREPORTER_H
