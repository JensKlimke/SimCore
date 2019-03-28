//
// Created by klimke on 25.03.2019.
//

#ifndef SIMULATION_FRAMEWORK_ISTORABLE_H
#define SIMULATION_FRAMEWORK_ISTORABLE_H


#define ADD(vector, name, structure)                        \
    vector.push_back(DataEntry{#name, &structure.name});    \


#include <vector>
#include <string>

namespace sim {

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

}; // namespace ::sim


#endif //SIMULATION_FRAMEWORK_ISTORABLE_H
