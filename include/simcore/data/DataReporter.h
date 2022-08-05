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

    typedef std::vector<std::string> KeyVector;
    typedef std::map<std::string, double> DataMap;
    typedef std::vector<DataMap> DataContainer;
    typedef std::function<bool(const DataMap &)> FilterFunction;

    /**
     * Constructor
     */
    DataReporter() {
        _keys.emplace_back("time");
        _keys.emplace_back("timeStepSize");
    };


    /**
     * Adds an double value to be added to the
     * @param val Pointer to the value
     * @param key Key to be used in json
     */
    void addValue(const std::string &key, const double *val) {
        _keys.emplace_back(key);
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

    DataContainer last() const {
        DataContainer data{};
        std::copy (_data.end() - 1, _data.end(), std::back_inserter(data));
        return data;
    }

    DataContainer filter(const FilterFunction &fnc) const {
        DataContainer data{};
        std::copy_if (_data.begin(), _data.end(), std::back_inserter(data), fnc);
        return data;
    }

    DataContainer all() const {
        DataContainer data{};
        std::copy (_data.begin(), _data.end(), std::back_inserter(data));
        return data;
    }

    void exportToCSV(std::ostream &out) {
        exportDataContainerToCSV(_data, _keys, out);
    }

    static void exportDataContainerToCSV(const DataContainer &container, const KeyVector &keys, std::ostream &out) {
        // export header line
        for(unsigned int i = 0; i < keys.size(); ++i)
            out << (i > 0 ? "," : "") << "\"" << keys[i] << "\"";
        // add new line
        out << std::endl;
        // export values
        for(auto &entry : container) {
            // create line of values
            for (unsigned int i = 0; i < keys.size(); ++i) {
                out << (i > 0 ? "," : "") << entry.at(keys[i]);
            }
            // add new line
            out << std::endl;
        }
    }

protected:

    std::map<std::string, const double*> _values{};
    KeyVector _keys{};
    DataContainer _data{};


};

#endif //SIMCORE_DATA_REPORTER_H
