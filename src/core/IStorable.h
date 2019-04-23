//
// Created by klimke on 25.03.2019.
//

#ifndef SIMCORE_ISTORABLE_H
#define SIMCORE_ISTORABLE_H


#define ADD(vector, name, structure)                          \
    vector.push_back(DataEntry{#name, &(structure).name});    \


#include <vector>
#include <string>

namespace sim {
namespace data {


class IStorable {

public:


    enum Context {
        PARAMETER, STATE, INPUT
    };


    struct DataEntry {
        std::string name;
        void *data;
    };


    /**
     * Returns a vector with data entries
     * @return Data entry vector
     */
    virtual std::vector<DataEntry> getData(Context context) = 0;

};

}} // namespace ::sim::data


#endif //SIMCORE_ISTORABLE_H
