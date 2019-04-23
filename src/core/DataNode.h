//
// Created by klimke on 17.04.2019.
//

#ifndef SIMCORE_DATANODE_H
#define SIMCORE_DATANODE_H

#include <string>
#include <unordered_map>
#include <vector>

namespace sim {
namespace data {

    struct DataValue {};


    template<typename T>
    struct Data : public DataValue {

        std::string name;
        T *value = nullptr;

    };


    struct DataVector : public DataValue {

        std::vector<DataValue> data;

    };


    struct DataNode {

        std::string name;
        std::unordered_map<std::string, DataNode> children;

        DataValue value;

    };


    template<typename T>
    void addValues(const std::vector<T> &data, DataNode &node) {

        // create DataVector

        // set vector as data
        node.value = data;

    }

}} // namespace ::sim::data

#endif //SIMCORE_DATANODE_H
