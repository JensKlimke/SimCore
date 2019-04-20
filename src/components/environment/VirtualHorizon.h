//
// Created by Jens Klimke on 2019-04-20.
//

#ifndef SIMCORE_VIRTUALHORIZON_H
#define SIMCORE_VIRTUALHORIZON_H

#include <core/SimModel.h>
#include <SimMap/lib_wrapper/odrfw.h>


class VirtualHorizon : public ::sim::IComponent {

    ::simmap::id_type_t _map_id = 0;

public:

    void initialize(double) override {

    }


    bool step(double simTime) override {

        return false;

    }


    void terminate(double) override {

    }


    /**
     * Registers a given map
     * @param map Map file to be registered
     */
    void registerMap(const std::string& map) {

        using namespace simmap;

        // if map is already registered, unregister
        if(_map_id != 0)
            unloadMap(_map_id);

        // load map
        if(loadMap(map.c_str(), &_map_id) != 0)
            throw std::runtime_error("Map could not be loaded.");

    }


    /**
     * Returns the map id
     * @return
     */
    unsigned int getMapId() const {

        return static_cast<unsigned int>(_map_id);

    }

};


#endif //SIMCORE_VIRTUALHORIZON_H
