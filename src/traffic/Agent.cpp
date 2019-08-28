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
    std::vector<const char*> rids(track.size());

    // write track data
    size_t i = 0;
    for(auto &rid : track)
        rids[i++] = rid.c_str();

    // set track
    auto err = simmap::setTrack(_id, rids.data(), track.size());
    if(err != 0)
        throw std::runtime_error("Could not set track.");

}



void Agent::setMapPosition(const std::string &edgeID, double s, double t) {

    // create map coordinate
    _map_pos.edgeID = edgeID.c_str();
    _map_pos.latPos = t;
    _map_pos.longPos = s;

    auto pl = getPathLengths();
    simmap::setMapPosition(getID(), _map_pos, &pl.first, &pl.second);

    // get absolute position and set
    simmap::getPosition(getID(), &_pos);

}



const Agent::MapPosition& Agent::setPosition(const Position &pos, double rMax) {

    // try to match agent
    auto err = simmap::match(getID(), pos, rMax, &_map_pos);
    if(err != 0)
        throw std::runtime_error("Matching not possible");

    // set to position and save absolute position
    setMapPosition(_map_pos.edgeID, _map_pos.longPos, _map_pos.latPos);
    _pos = pos;

    // return updated map position
    return _map_pos;

}



const Agent::Position & Agent::getPosition() const {

    return _pos;

}



const Agent::MapPosition & Agent::getMapPosition() const {

    // update map position and return
    simmap::getMapPosition(getID(), &_map_pos);
    return _map_pos;

}



void Agent::move(double ds, double t) {

    // move agent along the path
    auto pl = getPathLengths();
    simmap::move(getID(), ds, t, &pl.first, &pl.second);

    // get absolute position and set
    simmap::getPosition(getID(), &_pos);

}


void Agent::setDynamics(double v, double a) {

    _v = v;
    _a = a;

}


std::pair<double, double> Agent::getPathLengths() const {

    return {200.0, 50.0};

}



void Agent::setDimensions(double length, double width) {

    _length = length;
    _width = width;

}



Agent::AgentData Agent::getState() const {

    return {_v, _a, _length, _width};

}


std::vector<Agent::Target> Agent::getTargets() const {

    // define number of targets
    unsigned long n = MAX_NO_OF_TARGETS;
    std::vector<Target> ti(n);

    // get target information
    if(simmap::targets(getID(), ti.data(), &n) != 0)
        throw std::runtime_error("Could not generate target list");

    return std::vector<Target>(ti.begin(), std::next(ti.begin(), n));

}


std::vector<Agent::Lane> Agent::getLanes() const {

    // define number lanes
    unsigned long n = MAX_NO_OF_LANES;
    std::vector<Lane> li(n);

    // get lane information
    if(simmap::lanes(getID(), li.data(), &n) != 0)
        throw std::runtime_error("Could not generate lane list");

    // only return n elements
    return std::vector<Lane>(li.begin(), std::next(li.begin(), n));

}


std::vector<Agent::HorizonKnot> Agent::getHorizon(const std::vector<double> &steps) const {

    // instantiate vector
    unsigned long n = steps.size();
    std::vector<simmap::HorizonInformation> hor(n);

    // get horizon information
    if(simmap::horizon(getID(), steps.data(), hor.data(), n) != 0)
        throw std::runtime_error("Could not generate horizon list");

    // transform to coordinate system
    std::vector<HorizonKnot> horizon(n);
    for(size_t i = 0; i < hor.size(); ++i) {
        horizon[i].s   = hor[i].s;
        horizon[i].pos = global2local({hor[i].x, hor[i].y, 0.0, hor[i].psi, hor[i].kappa});
        horizon[i].egoLaneWidth   = hor[i].laneWidth;
        horizon[i].rightLaneWidth = hor[i].rightWidth;
        horizon[i].leftLaneWidth  = hor[i].leftWidth;
    }

    return horizon;

}


std::vector<Agent::RoadObject> Agent::getRoadObjects() const {

    // instantiate vector
    unsigned long n = MAX_NO_OF_OBJECTS;
    std::vector<simmap::ObjectInformation> obj(n);

    if(simmap::objects(getID(), obj.data(), &n) != 0)
        throw std::runtime_error("Could not generate object list");

    // resize vector
    return std::vector<RoadObject>(obj.begin(), std::next(obj.begin(), n));

}


std::vector<sim::data::IStorable::DataEntry> Agent::getData(sim::data::IStorable::Context context) const {

    using namespace sim::data;

    std::vector<sim::data::IStorable::DataEntry> ret{};
    if(context == sim::data::IStorable::Context::STATE) {

        // absolute position and state
        ret.push_back(createDataEntry("x",     &_pos.x));
        ret.push_back(createDataEntry("y",     &_pos.y));
        ret.push_back(createDataEntry("z",     &_pos.z));
        ret.push_back(createDataEntry("psi",   &_pos.phi));
        ret.push_back(createDataEntry("kappa", &_pos.kappa));
        ret.push_back(createDataEntry("v",     &_v));
        ret.push_back(createDataEntry("a",     &_a));

        // map position
        // ret.push_back(createDataEntry("edge", &_map_pos.edgeID));
        ret.push_back(createDataEntry("s",    &_map_pos.longPos));
        ret.push_back(createDataEntry("t",    &_map_pos.latPos));

    } else if(context == sim::data::IStorable::PARAMETER) {

        ret.push_back(createDataEntry("id",     &_id));
        ret.push_back(createDataEntry("width",  &_width));
        ret.push_back(createDataEntry("length", &_length));

    }

    return ret;

}


Agent::Position Agent::global2local(const Position &global) const {

    // pre-calculate relative position and trigonometric values
    auto dx = global.x - _pos.x;
    auto dy = global.y - _pos.y;
    auto c = cos(-_pos.phi);
    auto s = sin(-_pos.phi);

    // return relative position
    return {c * dx - s * dy, s * dx + c * dy, 0.0,global.phi - _pos.phi, global.kappa};

}

