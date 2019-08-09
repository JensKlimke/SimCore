//
// Created by klimke on 22.03.2019.
//

#ifndef SIMCORE_DATAMANAGER_H
#define SIMCORE_DATAMANAGER_H

#include <map>
#include <list>
#include <iostream>
#include <sstream>
#include "core/functions.h"
#include "core/IStorable.h"

namespace sim {
namespace data {



    /**
     * TODO: DataNode etc.
     */
    class DataManager {

        struct DataNode {

            std::shared_ptr<IDataSet> value;
            std::list<DataNode> nodeArray{};
            std::list<std::pair<std::string, DataNode>> nodeMap{};

        };

        DataNode _root{};
        std::map<std::string, const DataNode*> _index{};


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
        void registerStorable(const std::string &name, const IStorable &store) {

            // create data node for storable class
            _root.nodeMap.emplace_back(name, DataNode{});
            auto dn = &_root.nodeMap.back().second;

            // register parameters
            regStorable(name, &store, dn);

        }



        /**
         * Registers the given storable container vector to the data manager
         * @param name Name of the vector
         * @param stores Storable container vector
         */
        void registerStorableVector(const std::string &name, const std::vector<const IStorable*> &stores) {

            _root.nodeMap.emplace_back(name, DataNode{});
            auto &dr = _root.nodeMap.back().second;

            for(auto &store : stores) {

                // create array
                dr.nodeArray.emplace_back(DataNode{});
                auto dn = &dr.nodeArray.back();

                // register storable
                regStorable(name, store, dn);

            }



        }


        /**
         * Returns the pointer to the value
         * @param name Name of the value
         * @return Pointer to the value
         */
        const void* getValue(const std::string &name) {

            if(_index.find(name) == _index.end())
                throw std::invalid_argument(sim::fnc::string_format("No value \"%s\" defined", name.c_str()));

            return _index[name]->value->v();

        }


        /**
         * Resets the data manager
         */
        void reset() {

            _index.clear();

        }


        /**
         * Streams the data manager to the given out stream
         * @param os Out stream
         */
        std::ostream &streamTo(std::ostream &os) const {

            nodeToStream(os, _root);

            return os;

        }


    private:


        /**
         * Registers a storable container to the given data node
         * @param name Name of the container
         * @param store Container
         * @param dn Data node to be written to
         */
        void regStorable(const std::string &name, const IStorable *store, DataNode *dn) {

            // register parameters
            dn->nodeMap.emplace_back("parameter", DataNode{nullptr});
            auto vals = store->getData(IStorable::PARAMETER);
            for (auto &v : vals)
                regValue(dn->nodeMap.back().second, name, "parameter", v);

            // register inputs
            dn->nodeMap.emplace_back("input", DataNode{nullptr});
            vals = store->getData(IStorable::INPUT);
            for (auto &v : vals)
                regValue(dn->nodeMap.back().second, name, "input", v);

            // register states
            dn->nodeMap.emplace_back("state", DataNode{nullptr});
            vals = store->getData(IStorable::STATE);
            for (auto &v : vals)
                regValue(dn->nodeMap.back().second, name, "state", v);

        }


        /**
         * Registers a value to the data manager
         * @param dn The data node, the value shall be stored in
         * @param name Name of the owner object
         * @param context Context of the value
         * @param value Value object
         */
        void regValue(DataNode &dn, const std::string &name, const std::string &context, const IStorable::DataEntry &v) {

            // add to root
            dn.nodeMap.emplace_back(v.name, DataNode{v.data});

            // save to index
            _index[sim::fnc::string_format("%s.%s.%s", name.c_str(), context.c_str(), v.name.c_str())]
                = &dn.nodeMap.back().second;

        }


        /**
         * Recursive function to create a json string from a data node
         * @param os Stream to be streamed in
         * @param node Data node
         * @return The stream itself
         */
        static std::ostream &nodeToStream(std::ostream &os, const DataNode &node) {

            if(node.value) { // value (leaf)

                // add value to stream
                return node.value->s(os);

            } else if(!node.nodeMap.empty()) { // map

                os << "{";

                // iterate over map elements
                unsigned long i = 0;
                for (auto &p : node.nodeMap) {
                    os << (i++ > 0 ? "," : "") << "\"" << p.first << "\":";
                    nodeToStream(os, p.second);
                }

                os << "}";

            } else { // array

                os << "[";

                // iterate over array element
                unsigned long i = 0;
                for (auto &p : node.nodeArray) {
                    os << (i++ > 0 ? "," : "");
                    nodeToStream(os, p);
                }

                os << "]";

            }

            return os;

        }


    };




}} // namespace ::sim::data


#endif //SIMCORE_DATAMANAGER_H
