//
// Created by klimke on 22.03.2019.
//

#ifndef SIMULATION_FRAMEWORK_DATAMANAGER_H
#define SIMULATION_FRAMEWORK_DATAMANAGER_H

#include <unordered_map>
#include <iostream>
#include "IStorable.h"

namespace sim {

    class DataManager {

        std::unordered_map<std::string, void *> _values{};


    public:


        DataManager() = default;


        virtual ~DataManager() = default;


        /**
         * Registers the given storable to the data manager
         * @param name Name of the storable
         * @param store Storable to be added
         */
        void registerValues(const std::string &name, IStorable &store) {

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


        /**
         * Registers a value to the data manager
         * @param name Name of the value
         * @param value Value to be set
         */
        void registerValue(const std::string &name, void *value) {

            _values[name] = value;

        }


        /**
         * Returns the pointer to the value
         * @param name Name of the value
         * @return Pointer to the value
         */
        void* getValue(const std::string &name) {

            return _values.at(name);

        }


    };

};


#endif //SIMULATION_FRAMEWORK_SATAMANAGER_H
