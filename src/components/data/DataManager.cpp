//
// Created by Jens Klimke on 2019-04-22.
//

#include "DataManager.h"

namespace sim {
namespace data {

    void DataManager::registerValues(const std::string &name, IStorable &store) {

        // add storable itself
        registerValue(name, &store);

        // register parameters
        auto vals = store.getData(IStorable::PARAMETER);
        for (auto &v : vals)
            registerValue(name + ".parameter." + v.name, v.data);

        // register inputs
        vals = store.getData(IStorable::INPUT);
        for (auto &v : vals)
            registerValue(name + ".input." + v.name, v.data);

        // register states
        vals = store.getData(IStorable::STATE);
        for (auto &v : vals)
            registerValue(name + ".state." + v.name, v.data);

    }


    void DataManager::registerValue(const std::string &name, void *value) {

        _values[name] = value;

    }


    void *DataManager::getValue(const std::string &name) {

        return _values.at(name);

    }


    void DataManager::reset() {

        _values.clear();

    }


    void DataManager::streamTo(std::ostream &os) const {

        os << "{";

        for (const auto &entry : _values)
            os << "\"" << entry.first << "\":\"" << entry.second << "\"";

        os << "}";

    }

}} // namespace ::sim::data


std::ostream &operator<< (std::ostream& os, const sim::data::DataManager &dm) {

    dm.streamTo(os);
    return os;

}
