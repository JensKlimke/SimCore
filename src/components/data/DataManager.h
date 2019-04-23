//
// Created by klimke on 22.03.2019.
//

#ifndef SIMCORE_DATAMANAGER_H
#define SIMCORE_DATAMANAGER_H

#include <unordered_map>
#include <iostream>
#include "core/IStorable.h"
//#include "DataNode.h"

namespace sim {
namespace data {



    /**
     * TODO: DataNode etc.
     */
    class DataManager {

        std::unordered_map<std::string, void *> _values{};


    public:


        /**
         * Default constructor
         */
        DataManager() = default;


        /**
         * Default destructor
         */
        virtual ~DataManager() = default;


        /**
         * Registers the given storable to the data manager
         * @param name Name of the storable
         * @param store Storable to be added
         */
        void registerValues(const std::string &name, IStorable &store);


        /**
         * Registers a value to the data manager
         * @param name Name of the value
         * @param value Value to be set
         */
        void registerValue(const std::string &name, void *value);


        /**
         * Returns the pointer to the value
         * @param name Name of the value
         * @return Pointer to the value
         */
        void* getValue(const std::string &name);


        /**
         * Resets the data manager
         */
        void reset();


        /**
         * Streams the data manager to the given out stream
         * @param os Out stream
         */
        void streamTo(std::ostream &os) const;

    };

}} // namespace ::sim::data



std::ostream &operator<< (std::ostream& os, const sim::data::DataManager &dm);


#endif //SIMCORE_DATAMANAGER_H
