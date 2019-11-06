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
// Created by Jens Klimke on 2019-03-25
//

#ifndef SIMCORE_ISTORABLE_H
#define SIMCORE_ISTORABLE_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>


#define ADD(vector, name, structure)                                            \
    vector.emplace_back(sim::data::createDataEntry(#name, &(structure).name))   \

#define ADD_PTR(vector, name, structure)                                        \
    vector.emplace_back(sim::data::createDataEntry(#name, (structure).name))    \


#include <vector>
#include <string>

namespace sim {
namespace data {

    struct IDataSet {
        virtual std::ostream &s(std::ostream &os) const = 0;
        virtual const void *v() const = 0;
    };


    class IStorable {

    public:

        enum Context {
            PARAMETER, STATE, INPUT
        };


        struct DataEntry {
            std::string name;
            std::shared_ptr<IDataSet> data;
        };


        /**
         * Returns a vector with data entries
         * @return Data entry vector
         */
        virtual std::vector<DataEntry> getData(Context context) const = 0;

    };



    template<typename T>
    struct DataValue : public IDataSet {
        const T *value = nullptr;
        explicit DataValue(const T *v) : value(v) {}
        std::ostream &s(std::ostream &os) const override {
            os << *value;
            return os;
        }
        const void *v() const override { return value; }
    };


    struct StringValue : public DataValue<std::string> {
        explicit StringValue(const std::string *val) : DataValue<std::string>(val) {}
        std::ostream &s(std::ostream &os) const override {
            os << "\"" << (*value) << "\"";
            return os;
        }
    };


    template<typename T>
    inline IStorable::DataEntry createDataEntry(const std::string &name, const T *value) {

        return IStorable::DataEntry{name, std::make_shared<DataValue<T>>(value)};

    }

    inline IStorable::DataEntry createDataEntry(const std::string &name, const std::string *value) {

        return IStorable::DataEntry{name, std::make_shared<StringValue>(value)};

    }


}} // namespace ::sim::data


#endif //SIMCORE_ISTORABLE_H
