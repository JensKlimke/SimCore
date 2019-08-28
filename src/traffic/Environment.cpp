//
// Created by Jens Klimke on 2019-04-22.
//

#include <iostream>
#include "Environment.h"

Environment::~Environment() {

    clear();

}


void Environment::createAgent(Agent *agent, unsigned int id, const std::vector<std::string> &track) {

    // create agent
    agent->setID(id);

    // register agent to simulation map service
    auto err = simmap::registerAgent(id, _map_id);
    if(err != 0)
        throw std::invalid_argument("Agent could not be registered");

    // set track
    if(!track.empty())
        agent->setTrack(track);

    // save and index
    _agents.emplace_back(agent);
    _index[id] = agent;

}


Agent* Environment::getAgent(unsigned int id) {

    // return agent with given ID
    return _index.at(id);

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


const std::vector<Agent*>& Environment::getAgents() const {

    return _agents;

}


void Environment::clear() const {

    auto err = simmap::clear();

    if(err != 0)
        throw std::runtime_error("Couldn't clear simmap framework.");

}