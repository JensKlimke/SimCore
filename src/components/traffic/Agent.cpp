//
// Created by Jens Klimke on 2019-04-22.
//

#include "Agent.h"
#include <SimMap/lib.h>
#include <iostream>



void Agent::setID(unsigned int id) {

    _id = id;

}



unsigned int Agent::getID() const {

    return _id;

}



void Agent::setTrack(const std::vector<std::string> &track) {

    // create string array
    const char ** rids = new const char*[track.size()];

    // write track data
    size_t i = 0;
    for(auto &rid : track)
        rids[i++] = rid.c_str();

    // set track
    auto err = simmap::setTrack(_id, rids, track.size());
    if(err != 0)
        throw std::runtime_error("Could not set track.");

}



void Agent::setMapPosition(const std::string &edgeID, double s, double t) {

    // create map coordinate
    simmap::MapPosition pos;
    pos.edgeID = edgeID.c_str();
    pos.latPos = t;
    pos.longPos = s;

    auto pl = getPathLengths();
    simmap::setMapPosition(getID(), pos, &pl.first, &pl.second);

    // get absolute position and set
    simmap::getPosition(getID(), &_pos);

}



Agent::MapPosition Agent::setPosition(const Position &pos, double rMax) {

    // match position
    simmap::MapPosition mpos{};

    // try to match agent
    auto err = simmap::match(getID(), pos, rMax, &mpos);
    if(err != 0)
        throw std::runtime_error("Matching not possible");

    // set to position and save absolute position
    setMapPosition(mpos.edgeID, mpos.longPos, mpos.latPos);
    _pos = pos;

    // return position
    return mpos;

}



Agent::Position Agent::getPosition() const {

    return _pos;

}



Agent::MapPosition Agent::getMapPosition() const {

    Agent::MapPosition pos;
    simmap::getMapPosition(getID(), &pos);

    return pos;

}



void Agent::move(double ds, double t) {

    // move agent along the path
    auto pl = getPathLengths();
    simmap::move(getID(), ds, t, &pl.first, &pl.second);

    // get absolute position and set
    simmap::getPosition(getID(), &_pos);

}



std::pair<double, double> Agent::getPathLengths() const {

    return {200.0, 50.0};

}



void Agent::setDynamics(double velocity, double acceleration) {

    _velocity = velocity;
    _acceleration = acceleration;

}



void Agent::setDimensions(double length, double width) {

    _length = length;
    _width = width;

}