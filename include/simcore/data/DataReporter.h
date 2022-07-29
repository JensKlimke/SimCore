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


#ifndef SIMCORE_DATA_REPORTER_H
#define SIMCORE_DATA_REPORTER_H

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include "../IComponent.h"
#include "../exceptions.h"

class DataReporter : public simcore::IComponent {


public:

    typedef std::map<std::string, double> DataMap;
    typedef std::vector<DataMap> DataContainer;

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
     * Stores the values to the value vector
     * @param simTime
     * @param deltaTime
     */
    void step(double simTime, double deltaTime) override {
        // create map
        std::map<std::string, double> map;
        // add time and time step size
        map["time"] = simTime;
        map["timeStepSize"] = deltaTime;
        // iterate over values
        for(auto &e : _values)
            map[e.first] = *e.second;
        // create new value
        _data.emplace_back(map);
    }


    DataContainer filter(const std::function<bool(const DataMap &)> &fnc) const {
        DataContainer data{};
        std::copy_if (_data.begin(), _data.end(), std::back_inserter(data), fnc);
        return data;
    }


protected:

    std::map<std::string, const double*> _values{};
    std::vector<std::map<std::string, double>> _data{};


};

#endif //SIMCORE_DATA_REPORTER_H
