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


#ifndef SIMCORE_UNIT_H
#define SIMCORE_UNIT_H

#include <cmath>

/**
 * @brief A traffic unit interface.
 */
struct Unit {

    /** A class to store a three dimensional value. */
    struct Vector2 {
        double x; // The x element (in *m*)
        double y; // The y element (in *m*)
    };

    Vector2 position = {0.0, 0.0}; // The actual position (in *m*)
    Vector2 heading = {1.0, 0.0}; // The actual heading (in *m*)
    double velocity = 0.0; // The actual velocity (in *m/s*)
    double acceleration = 0.0; // The actual acceleration (in *m/s^2*)
    double yawRate = 0.0; // The actual yaw rate (in *rad/s*)
    double curvature = 0.0; // The actual curvature (in *1/m*)
    double distance = 0.0; // The actual distance travelled (in *m*)


    /**
     * Integrates the curvature and acceleration
     * @param dt
     */
    void integrate(double dt) {

        // velocity
        velocity += acceleration * dt;

        // limit velocity
        if(velocity <= 0.0)
            velocity = 0.0;

        // limit acceleration
        if(velocity <= 0.0 && acceleration <= 0.0)
            acceleration = 0.0;

        // calculate distance
        auto ds = velocity * dt;
        distance += ds;

        // set yaw rate
        yawRate = curvature * velocity;

        // calculate change in position
        auto dSin = sin(yawRate * dt);
        auto dCos = cos(yawRate * dt);

        // calculate heading
        heading = {
                heading.x * dCos - heading.y * dSin,
                heading.y * dCos + heading.x * dSin
        };

        // normalize (to avoid cumulated accuracy errors)
        auto l = 1.0 / sqrt(heading.x * heading.x + heading.y * heading.y);
        heading.x *= l;
        heading.y *= l;

        // position
        position.x += heading.x * ds;
        position.y += heading.y * ds;

    }


    /**
     * Converts the angle value into a heading vector
     * @param angle Angle to be converted (east is 0, to north is positive, in rad)
     * @return The heading vector normalized to a length of 1
     */
    static Vector2 angle2heading(double angle) {
        return {cos(angle), sin(angle)};
    }


    /**
     * Converts the heading vector into a angle
     * @param heading Heading vector
     * @return The angle (east is 0, to north is positive, in rad)
     */
    static double heading2angle(const Vector2 &heading) {
        return atan2(heading.y, heading.x);
    }


    /**
     * Returns the distance since last time requested
     * @return The distance travelled since last request
     */
    double getDistance() {
        auto ds = distance - _distance;
        _distance = distance;
        return ds;
    }


    /**
     * Calculates the given global position into the vehicle coordinate system
     * @param global Global position to be transferred into vehicle CS
     * @return The coordinate in the local coordinate system
     */
    Vector2 toLocal(const Vector2 &global) const {

        // transform
        return coordinateTransform(global, position, heading);

    }


    /**
     * Calculates the given global angle into the vehicle coordinate system
     * @param global Global heading angle
     * @return The angle in the local coordinate system
     */
    double toLocalAngle(double angle) const {

        // get heading of point
        auto h = angle2heading(angle);

        // move to vehicle's origin
        h.x += position.x;
        h.y += position.y;

        // rotate into local CS
        h = toLocal(h);

        // calculate angle
        return heading2angle(h);

    }


    /**
     * Transforms the given coordinate into the given coordinate system (origin, heading)
     * @param fromPosition Coordinate to be transformed
     * @param toOrigin Origin of the target coordinate system
     * @param toHeading Heading of the target coordinate system
     * @return The position in the local coordinate system
     */
    static Vector2 coordinateTransform(const Vector2 &fromPosition, const Vector2 &toOrigin, const Vector2 &toHeading) {

        // create target position
        Vector2 toPosition = fromPosition;

        // transpose
        toPosition.x -= toOrigin.x;
        toPosition.y -= toOrigin.y;

        // rotate
        return {
                toHeading.x * toPosition.x + toHeading.y * toPosition.y,
                -toHeading.y * toPosition.x + toHeading.x * toPosition.y
        };

    }





protected:

    double _distance;

};

#endif // SIMCORE_UNIT_H
