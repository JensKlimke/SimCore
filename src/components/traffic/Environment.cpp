//
// Created by Jens Klimke on 2019-04-22.
//

#include "Environment.h"
#include <iostream>


Agent * Environment::createAgent(unsigned int id, const std::vector<std::string> &track) {

    // create agent
    auto ag = new Agent(id);

    // register agent to simulation map service
    auto err = simmap::registerAgent(id, _map_id);
    if(err != 0)
        throw std::runtime_error("Agent could not be registered");

    // set track
    if(!track.empty())
        ag->setTrack(track);

    // save and index
    _agents.emplace_back(std::unique_ptr<Agent>(ag));
    _index[id] = ag;

    // return agent
    return ag;

}


Agent* Environment::getAgent(unsigned int id) {

    // return agent with given ID
    return _index.at(id);

}


void Environment::initialize(double) {

}


bool Environment::step(double simTime) {

    return false;

}


void Environment::terminate(double) {

}


void Environment::registerMap(const std::string& map) {

    using namespace simmap;

    // if map is already registered, unregister
    if(_map_id != 0)
        unloadMap(_map_id);

    // load map
    auto err = loadMap(map.c_str(), &_map_id);
    if(err != 0)
        throw std::runtime_error("Map could not be loaded.");

}


unsigned int Environment::getMapID() const {

    return static_cast<unsigned int>(_map_id);

}


void Environment::clear() const {

    simmap::clear();

}