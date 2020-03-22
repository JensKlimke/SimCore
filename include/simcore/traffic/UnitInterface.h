// Copyright (c) 2020 Jens Klimke <jens.klimke@rwth-aachen.de>. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// 
// Created by Jens Klimke on 2020-03-22.
//


#ifndef UNIT_INTERFACE_H
#define UNIT_INTERFACE_H



/**
 * @brief A traffic unit interface.
 */
class UnitInterface {

public:

    /** A class to store a three dimensional value. */
    struct Vector3 {
        double x; // The x element (in *m*)
        double y; // The y element (in *m*)
        double z; // The z element (in *m*)
    };

    /** A class to store the states. */
    struct State {
        Vector3 position = {0.0, 0.0, 0.0}; // The actual position (in *m*)
        double velocity = 0.0; // The actual velocity (in *m/s*)
        double acceleration = 0.0; // The actual acceleration (in *m/s^2*)
        double yawAngle = 0.0; // The actual yaw angle (in *rad*)
        double yawRate = 0.0; // The actual yaw rate (in *rad/s*)
        double wheelAngle = 0.0; // The actual wheel steer angle (in *rad*)
    };

    /** A class to store the parameters. */
    struct Parameters {
        double wheelBase = 3.0; // The wheel base (in *m*)
        Vector3 size = {5.0, 2.2, 1.5}; // The size of the vehicle
        Vector3 driverPosition = {0.5, 0.5, 1.1}; // The position of the driver related to the center
    };



protected:

    unsigned long id = 0; // A unique ID for the unit.
    State state{}; // The state of the unit.
    Parameters parameters{}; // The parameters of the unit.


public:

    /** Default constructor */
    UnitInterface() = default;


    /** Default destructor */
    virtual ~UnitInterface() = default;


    /** This method resets the vehicle memory */
    virtual void reset() = 0;

    /**
    * Returns the ID the model
    * @return The ID
    */
    virtual unsigned long getID() const {
        return id;
    }


    /**
    * Returns the pointer for the ID structure of the model
    * @return The ID pointer
    */
    virtual unsigned long *getID_ptr() {
        return &id;
    }


    /**
    * Returns the const pointer for the ID structure of the model
    * @return The const ID point
    */
    virtual const unsigned long *getID_ptr() const {
        return &id;
    }


    /**
    * Returns the pointer for the state structure of the model
    * @return The state pointer
    */
    virtual State *getState() {
        return &state;
    }


    /**
    * Returns the const pointer for the state structure of the model
    * @return The const state point
    */
    virtual const State *getState() const {
        return &state;
    }


    /**
    * Returns the pointer for the parameters structure of the model
    * @return The parameters pointer
    */
    virtual Parameters *getParameters() {
        return &parameters;
    }


    /**
    * Returns the const pointer for the parameters structure of the model
    * @return The const parameters point
    */
    virtual const Parameters *getParameters() const {
        return &parameters;
    }


};

#endif // UNIT_INTERFACE_H
