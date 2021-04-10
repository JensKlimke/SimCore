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


/**
 * @brief A traffic unit interface.
 */
struct Unit {

    /** A class to store a three dimensional value. */
    struct Vector3 {
        double x; // The x element (in *m*)
        double y; // The y element (in *m*)
        double z; // The z element (in *m*)
    };

    Vector3 position = {0.0, 0.0, 0.0}; // The actual position (in *m*)
    Vector3 heading = {1.0, 0.0, 0.0}; // The actual heading (in *m*)
    double velocity = 0.0; // The actual velocity (in *m/s*)
    double acceleration = 0.0; // The actual acceleration (in *m/s^2*)
    double yawRate = 0.0; // The actual yaw rate (in *rad/s*)
    double curvature = 0.0; // The actual curvature (in *1/m*)
    double distance = 0.0; // The actual distance travelled (in *m*)
    double steering = 0.0; // The actual steering (no unit, -1: full right, 0: straight, 1: full left)
    double pedal = 0.0; // The actual pedal value (no unit, -1..0: braking, 0..1 throttle)


    /**
     * Converts the angle value into a heading vector
     * @param angle Angle to be converted (east is 0, to north is positive, in rad)
     * @return The heading vector normalized to a length of 1
     */
    static Vector3 angle2heading(double angle) {
        return {cos(angle), sin(angle), 0.0};
    }


    /**
     * Converts the heading vector into a angle
     * @param heading Heading vector
     * @return The angle (east is 0, to north is positive, in rad)
     */
    static double heading2angle(const Vector3 &heading) {
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
    Vector3 toLocal(const Vector3 &global) const {

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
    static Vector3 coordinateTransform(const Vector3 &fromPosition, const Vector3 &toOrigin, const Vector3 &toHeading) {

        // create target position
        Vector3 toPosition = fromPosition;

        // transpose
        toPosition.x -= toOrigin.x;
        toPosition.y -= toOrigin.y;
        toPosition.z -= toOrigin.z;

        // rotate
        return {
             toHeading.x * toPosition.x + toHeading.y * toPosition.y,
            -toHeading.y * toPosition.x + toHeading.x * toPosition.y,
             toPosition.z
        };

    }


protected:

    double _distance;

};

#endif // SIMCORE_UNIT_H
