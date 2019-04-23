//
// Created by Jens Klimke on 2019-04-20.
//

#ifndef SIMCORE_AGENT_H
#define SIMCORE_AGENT_H

#include <vector>
#include <string>
#include <cmath>
#include <SimMap/lib_wrapper/odrfw.h>


#ifndef VEH_DEFAULT_LENGTH
#define VEH_DEFAULT_LENGTH 5.0;
#endif

#ifndef VEH_DEFAULT_WIDTH
#define VEH_DEFAULT_WIDTH 2.2;
#endif


class Agent {

    unsigned int _id = 0;
    simmap::Position _pos{};

    double _length = VEH_DEFAULT_LENGTH;
    double _width  = VEH_DEFAULT_WIDTH;

    double _velocity = 0.0;
    double _acceleration = 0.0;


public:

    typedef simmap::Position Position;

    explicit Agent(unsigned int id);
    virtual ~Agent() = default;


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
     * @param roadID Road ID
     * @param laneID Lane ID
     * @param s Longitudinal position in road
     * @param t Relative lateral position from lane center
     */
    void setMapPosition(const std::string &roadID, int laneID, double s, double t);


    /**
     * Sets the absolute position of the agent
     * @param pos Position to be set
     * @param rMax Maximum moved distance from last positioning
     */
    void setPosition(const Position &pos, double rMax);


    /**
     * Returns the position of the agent
     * @return Position of the agent
     */
    Position getPosition() const;


    /**
     * Moves the agent along the road by distance *\Delta s* and to the lateral offset *t*
     * @param ds Distance to be moved
     * @param t Lateral offset to be set
     */
    void move(double s, double t);


    /**
     * Sets the current velocity and acceleration of the agents vehicle
     * @param velocity Velocity to be set
     * @param acceleration Acceleration to be set
     */
    void setDynamics(double velocity, double acceleration);


    /**
     * Sets the dimensions of the agents vehicle
     * @param length Length to be set
     * @param width Width to be set
     */
    void setDimensions(double length, double width);



protected:

    virtual std::pair<double, double> getPathLengths() const;


};


#endif //SIMCORE_AGENT_H
