//
// Created by Jens Klimke on 2019-04-20.
//

#ifndef SIMCORE_AGENT_H
#define SIMCORE_AGENT_H

#include <core/IComponent.h>
#include <map>
#include <cmath>

class Agent : public sim::IComponent {

    unsigned int _id = 0;
    static std::map<unsigned int, Agent*> _index;


public:

    Agent() = default;
    ~Agent() override = default;


    /**
     * Register agent
     * @param id ID of the agent
     */
    void create(unsigned int id);


    /**
     * Sets the track for the agent
     * @param track Track to be set
     */
    void setTrack(const std::vector<std::string> &track);


    /**
     * Sets the position of the agent onto the given map
     * @param roadID Road ID
     * @param laneID Lane ID
     * @param s Longitudinal position in road
     * @param t Relative lateral position from lane center
     */
    void setPosition(const std::string &roadID, int laneID, double s, double t);


    /**
     * Sets the absolute position of the agent
     * @param x x-position
     * @param y y-position
     * @param z z-position
     * @param rMax Maximum moved distance from last positioning
     */
    void setPosition(double x, double y, double z, double rMax = INFINITY);



    void initialize(double initTime) override;

    bool step(double simTime) override;

    void terminate(double simTime) override;


};


#endif //SIMCORE_AGENT_H
