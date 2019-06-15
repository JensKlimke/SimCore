//
// Created by Jens Klimke on 2019-04-20.
//

#ifndef SIMCORE_AGENT_H
#define SIMCORE_AGENT_H

#include <vector>
#include <string>
#include <cmath>
#include <SimMap/lib.h>
#include <core/IStorable.h>

#ifndef VEH_DEFAULT_LENGTH
#define VEH_DEFAULT_LENGTH 5.0;
#endif

#ifndef VEH_DEFAULT_WIDTH
#define VEH_DEFAULT_WIDTH 2.2;
#endif


class Agent : public sim::data::IStorable {

    static const unsigned long NO_OF_TARGETS = 64;
    static const unsigned long NO_OF_LANES   = 32;

    unsigned int _id = 0;
    simmap::Position _pos{};

    double _length = VEH_DEFAULT_LENGTH;
    double _width  = VEH_DEFAULT_WIDTH;


public:

    typedef simmap::Position Position;
    typedef simmap::MapPosition MapPosition;


    Agent() = default;
    virtual ~Agent() = default;



    /**
     * Sets the ID of the agent
     * @param id Agent ID
     */
    void setID(unsigned int id);


    /**
     * Returns the ID of the agent
     * @return ID
     */
    unsigned int getID() const;


    /**
     * Sets the track for the agent
     * @param track Track to be set
     */
    void setTrack(const std::vector<std::string> &track);


    /**
     * Sets the position of the agent onto the given map
     * @param edgeID Name of the edge
     * @param s Longitudinal position in road
     * @param t Relative lateral position from lane center
     */
    void setMapPosition(const std::string &edgeID, double s, double t);


    /**
     * Sets the absolute position of the agent
     * @param pos Position to be set
     * @param rMax Maximum moved distance from last positioning
     */
    MapPosition setPosition(const Position &pos, double rMax);


    /**
     * Returns the position of the agent
     * @return Position of the agent
     */
    Position getPosition() const;


    /**
     * Returns the current map position of the agent
     * @return Map position
     */
    MapPosition getMapPosition() const;


    /**
     * Moves the agent along the road by distance *\Delta s* and to the lateral offset *t*
     * @param ds Distance to be moved
     * @param t Lateral offset to be set
     */
    void move(double ds, double t);


    /**
     * Sets the dimensions of the agents vehicle
     * @param length Length to be set
     * @param width Width to be set
     */
    void setDimensions(double length, double width);


    /**
     * Returns a list of targets. The number of returned (and ordered) targets can be set by macro NO_OF_TARGETS
     * @return List of targets
     */
    std::vector<simmap::TargetInformation> getTargets();


    /**
     * Returns lane information around the agent
     * @return Lane information vector
     */
    std::vector<simmap::LaneInformation> getLanes();


    /**
     * Return data
     * @param context
     * @return
     */
    std::vector<sim::data::IStorable::DataEntry> getData(sim::data::IStorable::Context context) const;


protected:

    virtual std::pair<double, double> getPathLengths() const;


};


#endif //SIMCORE_AGENT_H
