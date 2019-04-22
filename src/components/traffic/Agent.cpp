//
// Created by Jens Klimke on 2019-04-22.
//

#include "Agent.h"
#include <SimMap/lib_wrapper/odrfw.h>

std::map<unsigned int, Agent*> Agent::_index{};



void create(unsigned int id) {

    // TODO: id

    auto err = simmap::registerAgent(id, _map_id);
    if(err != 0)
    throw std::runtime_error("Agent could not be registered");

}



void Agent::setTrack(const std::vector<std::string> &track) {

    // create string array
    const char ** rids = new const char*[track.size()];

    // write track data
    unsigned long i = 0;
    for(auto rid : track) {
        rids[i] = track.at(i).c_str();
        i++;
    }

    // set track
    auto err = simmap::setTrack(_id, rids, track.size());
    if(err != 0) {
        std::cout << err << std::endl;
        throw std::runtime_error("Could not set track.");
    }

}



void Agent::setPosition(const std::string &roadID, int laneID, double s, double t) {

    // TODO: set map position

}



void Agent::setPosition(double x, double y, double z, double rMax) {

    // TODO: set absolute position and match

}



void Agent::initialize(double initTime) override {



}



bool Agent::step(double simTime) override {

    return true;

}



void Agent::terminate(double simTime) override {



}