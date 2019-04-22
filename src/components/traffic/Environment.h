//
// Created by Jens Klimke on 2019-04-20.
//

#ifndef SIMCORE_ENVIRONMENT_H
#define SIMCORE_ENVIRONMENT_H

#include <core/Model.h>
#include <SimMap/lib_wrapper/odrfw.h>
#include <memory>
#include "Agent.h"


class Environment : public ::sim::IComponent {

    ::simmap::id_type_t _map_id = 0;
    std::vector<std::unique_ptr<Agent>> _agents{};

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
        auto err = loadMap(map.c_str(), &_map_id);
        if(err != 0)
            throw std::runtime_error("Map could not be loaded.");

    }


    /**
     * Returns the map id
     * @return
     */
    unsigned int getMapId() const {

        return static_cast<unsigned int>(_map_id);

    }



    /**
     * Registers an agent to the map framework
     * @param id ID of the agent
     * @param track Track of the agent coded as a vector of road IDs with a minus for backward direction
     */
    void registerSimAgent(unsigned int id, const std::vector<std::string> &track = {}) {

        // add agent
        auto ag = new Agent();

        // register agent
        ag->register();

        if(!track.empty())
            ag->setTrack(track);

        _agents.emplace_back(ag);

    }


    /**
     * Clears all maps and agents from the map framework
     */
    void clear() const {

        simmap::clear();

    }


};


#endif //SIMCORE_ENVIRONMENT_H
