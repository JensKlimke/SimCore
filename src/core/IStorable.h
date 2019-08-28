//
// Created by klimke on 25.03.2019.
//

#ifndef SIMCORE_ISTORABLE_H
#define SIMCORE_ISTORABLE_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>


#define ADD(vector, name, structure)                                         \
    vector.emplace_back(sim::data::createDataEntry(#name, &(structure).name))   \

#define ADD_PTR(vector, name, structure)                                     \
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
