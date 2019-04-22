//
// Created by Jens Klimke on 2019-04-20.
//

#ifndef SIMCORE_AGENT_H
#define SIMCORE_AGENT_H

#include <core/IComponent.h>

class Agent : public sim::IComponent {

    unsigned int _id = 0;


public:

    Agent() = default;
    ~Agent() = default;


    /**
     * Sets the track for the agent
     * @param track
     */
    void setTrack(const std::vector<std::string> &track) {

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
        if(err != 0)
            throw std::runtime_error("Could not set track.");

    }


    /**
     * Sets the position of the agent onto the given map
     * @param roadID Road ID
     * @param laneID Lane ID
     * @param s Longitudinal position in road
     * @param t Relative lateral position from lane center
     */
    void setPosition(const std::string &roadID, int laneID, double s, double t) {

        // TODO: set map position

    }


    void setPosition(double x, double y, double z) {

        // TODO: set absolute position and match

    }


    void initialize(double initTime) override {



    }


    bool step(double simTime) override {


        return true;

    }


    void terminate(double simTime) override {

    }

};


#endif //SIMCORE_AGENT_H
