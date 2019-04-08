//
// Created by Jens Klimke on 2019-03-23.
//

#ifndef SIMULATION_FRAMEWORK_AGENTMODELINTERFACE_H
#define SIMULATION_FRAMEWORK_AGENTMODELINTERFACE_H

#include <stdint.h>

namespace agmod {


    /*! \mainpage
     * This documentation describes the ika/fka driver model interface which consists of the following structs:
     * * \ref EgoPerception - Signals which describe the state of the driver/vehicle unit
     * * \ref Target - Information about surrounding targets
     * * \ref Environment - Description of the ego environment
     * * \ref PrimaryControls - The drivers reaction regarding the vehicle control (pedals, steering, ...)
     * * \ref SecondaryControls - The drivers reaction regarding secondary inputs of the vehicle (lights, indicators, ...)
     *
     * The mentioned structs can are organized in inputs and outputs on two levels (Logics, Control)
     * * \ref InputLogics - Inputs for the logics module of the driver model
     * * \ref InputControl - Inputs for the control module of the driver model
     * * \ref OutputLogics - Outputs of the logics module of the driver model
     * * \ref OutputControl - Outputs of the control module of the driver model
     *
     * \section todo TODO interface KK:
     * * Define coordinate systems (e.g. vehicle one)
     * * Refer to vehicle coordinate system e.g. in ego interface
     *
     * \section csys Coordinate systems
     * \subsection veh-csys Vehicle coordinate system
     * Usual vehicle coordinate system:
     * * _x_ describes longitudinal vehicle direction
     * * _y_ describes the orthogonal (lateral) vehicle direction
     *
     * See also \ref q-and-u
     *
     * TODO: describe more precisely with image
     *
     * \subsection road-csys Road coordinate system
     * The road coordinate system is defined like this:
     * * _s_ is the process variable along the path (see OpenDRIVE geometric reference)
     * * _d_ is the lateral distance (orthogonal) to the reference line (e.g. center of lane)
     * * _psi_ and _dPsi_ are the relative angle and relative angle rate between vehicle x-direction and road s-direction
     * * _ds_ describes the distance from the reference point to the considered point along the path (_s_)
     *
     * See also \ref q-and-u
     *
     * TODO: descibe more precisely with image
     *
     * \page q-and-u Quantities and Units
     * This page describes the quantities and units used in the interface. In general SI-units are used.
     *
     *
     * Quantity                    | Unit              | Description
     * --------------------------- | ----------------- | -------------
     * Distance                    | m                 | n/a
     * Distance along road         | m                 | n/a
     * Velocity                    | m / s             | n/a
     * Acceleration                | m / s<sup>2</sup> | n/a
     * Absolute angle              | - (radiant)       | Angle relative to the worlds x-axis
     * Relative angle              | - (radiant)       | Angle between two axes,
     *
     *
     * \page lane-coding Lane Coding
     * Lanes are coded relative to the ego lane (ID 0). Lanes to the right are coded by negative IDs
     * decreasing from the nearest lane the the farest lane. Lane to the left are coded by positive
     * IDs increasing from the nearest lane to the farest lane.
     *
     * See also OpenDRIVE.
     *
     */


    // constants
    static const unsigned int NOT  = 16; //!< Number of targets passed to the driver model
    static const unsigned int NOH  = 32; //!< Number of horizon points (along the road) passed to the driver model
    static const unsigned int NOS  = 32; //!< Number of signals passed to the driver model
    static const unsigned int NOL  = 32; //!< Number of lanes passed to the driver model
    static const unsigned int NORP = 2;

    /*! This enum describes a access state of an area.
    */
    enum Accessibility {ACC_ACCESSIBLE, ACC_NOT_ALLOWED, ACC_NOT_ACCESSIBLE, ACC_NOT_SET} ;


    /*! This enum describes the driving direction of the area
     */
    enum DrivingDirection {DD_FORWARDS, DD_BACKWARDS, DD_BOTH, DD_NONE, DD_NOT_SET};


    /*! This enum describes the type of a signal
     */
    enum SignalType {SIGNAL_STOP, SIGNAL_TLS, SIGNAL_SPEED_LIMIT, SIGNAL_NOT_SET};


    // repeatable types
    struct Point2D {
        double x;
        double y;
    };

    // value in a distance
    struct Point {
        double ds;
        double value;
    };


    /*! \brief Interface for ego vehicle perception
     * Signals which describe the state of the vehicle
     */
    struct EgoPerception {
        Point2D v;         //!< Velocity (in *m/s*) in vehicle x and y direction (see \ref veh-csys)
        Point2D a;         //!< Acceleration (in *m/s^2*) in vehicle x and y direction
        double psi;        //!< Relative yaw angle (in *rad* math. positive between road tangent direction and vehicle x-direction, see \ref veh-csys)
        double dPsi;       //!< Yaw rate (in *rad/s*, derivative of yaw angle, see \ref veh-csys)
        double s;          //!< Distance, the vehicle travelled since the last reset (see \ref veh-csys)
        double d;          //!< Lateral offset to the lane center
    };


    /*! \brief Width values for lanes (ego, left lane, right lane)
     */
    struct LaneDef {
        double ego;    //!< Width of the ego lane (in *m*)
        double left;   //!< Width of the left lane (in *m*)
        double right;  //!< Width of the right lane (in *m*)
    };


    /*! \brief This struct represents a knot of the **virtual horizon**
     */
    struct HorizonKnot {
        double ds;        //!< distance \p s along reference line
        Point2D xy;       //!< x and y co-ordinate relative to ego unit
        double psi;       //!< heading relative to vehicle x-axis
        double kappa;     //!< curvature of the road
        LaneDef width;    //!< widths of
    };


    /*! \brief Information about each existing **lane** is described by 6 values
     */
    struct EnvLane {
        int8_t  id;           //!< Unique id coded by \ref lane-coding. Default value for invalid/not existing lanes is \p 127
        double  width;        //!< Width of the lane in [m]
        double  route;        //!< Length along the route of the driver in [m]. Attention: lanes do not end, when roads end, all connected lanes are considered as one long lane. When a lane is closed, the lane probably still belongs to the route.
        double  closed;       //!< Next closed point on lane. At this point the driver has to leave the lane latest even if it belongs to the route. Also add lane endings here.
        DrivingDirection dir; //!< The driving direction of the lane
        Accessibility access; //!< Accessibility of the lane
    };


    /*! \brief Information about each existing **signal** is described by 4 values
     */
    struct EnvSignal {
        uint32_t   id;    //!< Unique id. Default value for invalid/not existing signal is \p 0. This value is necessary for the memory of the driver. signs could be coded by 1000 * road_id + sequence_no_of_sign
        double     ds;    //!< Distance to the sign. Attention: The actual distance to the signal is not relevant.
        SignalType type;  //!< Type of the signal
        int32_t    value; //!< Value of the signal
    };


    /*! \brief Environment information.
     *
     * The environment is defined by a virtual horizon, the surrounding lanes and signs
     */
    struct Environment {
        HorizonKnot   horizon[NOH];      //!< Array for virtual horizon knots along \p s
        EnvLane       lanes[NOL];        //!< Array for lanes on the road
        EnvSignal     signals[NOS];      //!< Array for all signals along road
    };








    /*! \brief Interface for the conscious guidance values
     * Signals which store the conscious guidance values of the driver model. These values can be used to manipulate the
     * driver's general behavior. The values describe an inner state of the driver an can be understood as the wish.
     * The model tries to reach this values if the situation allows it.
     */
    struct Conscious {
        double  vDes;        //!< Desired velocity (in *m/s*). The speed can only be reached if no further influences force the driver to drive slower.
        double  vMin;        //!< Minimum velocity (in *m/s*). The value describes the speed a driver accepts when performing manoeuvres, such as dropping back when changing lanes.
        Point   vAntic;      //!< Anticipated velocity step ahead. //TODO
        double  dsFollow;    //!< The desired following distance for the relevant target (*m*)
        double  dsStop;      //!< Stop distance (in *m*). This value describes the distance the driver stops in front of a stop line or another point. The distance is measured relative to the _front of the vehicle_ (NOT THE DISTANCE TO THE DRIVER'S POSITION)
        double  stopped;     //!< The time since the driver has stopped
        Point2D xyRef[NORP]; //!< Reference points (in *m*). These points describe reference points the driver uses to control the vehicle laterally
        double  latOffset;   //!< Lateral desired offset (in *m*). The value describes the desired lateral offset from the lane center which the driver wants to reach locally.
    };



    /*! \brief Interface for the sub-conscious guidance values
     * Signals which store the sub-conscious guidance values of the driver model. These values can be used to implement
     * a microscopic behavior directly or to be fed into a pedal and steering controller to implement sub-microscopic
     * behavior with a component-based vehicle model.
     */
    struct SubConscious {
        double aDes;     //!< Desired acceleration (in *m/s^2*)
        double dPsiDes;  //!< Desired yaw rate (in *1/s*, radiant per second)
    };



    /*! \brief Parameters for uninfluenced driving (cruising)
     * This structure stores the parameters for longitudinal cruising (velocity control)
     */
    struct ParamCruise {
        double thwMax;    //!< The maximum time headway the driver starts to react (in *s*)
        double thwMin;    //!< The time headway ahead of the speed change the driver controls to (in *s*)
        double delta;     //!< The power for the local speed reaction (see \delta in IDM: https://en.wikipedia.org/wiki/Intelligent_driver_model)
        double deltaPred; //!< The power for the predictive speed reaction
        double aScale;    //!< Factor for buckling of vel curve
        double a;         //!< The maximum acceleration (in *m/s^2*)
        double b;         //!< The maximum deceleration (in *m/s^2*)
        double vScale;    //!< A scale factor to scale up or down the speed limit
        double ayMax;     //!< Maximum lateral acceleration (in *m/s^2*)
        double vMax;      //!< Maximum personal velocity (in *m/s*)
    };


    /*! \brief Parameters for influenced driving (e. g. following, stopping)
     * This structure stores the parameters for longitudinal influenced driving (distance control)
     */
    struct ParamFollow {
        double timeHeadway; //!< The time headway the driver tries to reach during following (in *s*)
        double dsStopped;   //!< The distance to the controlled target when stopped
    };



    /*! \brief Parameters of the vehicle
     *
     */
    struct ParamVehicle {
        Point2D size;      //!< Size of the ego vehicle
        Point2D pos;       //!< The driver's position referenced to the center of the vehicle box defined by *size*
    };


};


#endif //SIMULATION_FRAMEWORK_AGENTMODELINTERFACE_H
