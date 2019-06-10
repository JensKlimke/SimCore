//
// Created by klimke on 25.03.2019.
//

#ifndef SIMCORE_ISTORABLE_H
#define SIMCORE_ISTORABLE_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>


#define ADD(vector, name, structure)                                                    \
    vector.push_back(DataEntry{#name, sim::data::createDataSet(&(structure).name)})     \

#define ADD_PTR(vector, name, structure)                                                \
    vector.push_back(DataEntry{#name, sim::data::createDataSet((structure).name)})      \


#include <vector>
#include <string>

namespace sim {
namespace data {


struct IDataSet {
    virtual std::ostream &s(std::ostream &os) const = 0;
    virtual const void *v() const = 0;
};


template<typename T>
struct DataSet : public IDataSet {
    const T *value = nullptr;
    explicit DataSet(const T *v) : value(v) {}
    std::ostream &s(std::ostream &os) const override {
        os << *value;
        return os;
    }
    const void *v() const override { return value; }
};


struct StringSet : public DataSet<std::string> {
    explicit StringSet(const std::string *val) : DataSet<std::string>(val) {}
    std::ostream &s(std::ostream &os) const override {
        os << "\"" << (*value) << "\"";
        return os;
    }
};


template<typename T>
inline std::shared_ptr<IDataSet> createDataSet(T *value) {

    return std::make_shared<DataSet<T>>(value);

}

inline std::shared_ptr<IDataSet> createDataSet(std::string *value)  {

    return std::make_shared<StringSet>(value);

}


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
    virtual std::vector<DataEntry> getData(Context context) = 0;

};

}} // namespace ::sim::data


#endif //SIMCORE_ISTORABLE_H
