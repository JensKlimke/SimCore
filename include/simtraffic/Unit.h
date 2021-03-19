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
    double velocity = 0.0; // The actual velocity (in *m/s*)
    double acceleration = 0.0; // The actual acceleration (in *m/s^2*)
    double yawAngle = 0.0; // The actual yaw angle (in *rad*)
    double yawRate = 0.0; // The actual yaw rate (in *rad/s*)
    double curvature = 0.0; // The actual curvature (in *1/m*)
    double distance = 0.0; // The actual distance travelled (in *m*)

};

#endif // SIMCORE_UNIT_H
