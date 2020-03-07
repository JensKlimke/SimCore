// Copyright (c) 2019 Jens Klimke <jens.klimke@rwth-aachen.de>. All rights reserved.
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
// Created by Jens Klimke on 29.11.2019.
//


#ifndef SIMDRIVE_DRIVERCONTROLLER_H
#define SIMDRIVE_DRIVERCONTROLLER_H

#include "PrimaryController.h"

class DriverController {

public:

    PrimaryController pedal;
    PrimaryController steerYawRate;
    PrimaryController steerCurv;

    PrimaryController::State *pState = nullptr;
    PrimaryController::State *sYawrateState = nullptr;
    PrimaryController::State *sCurvState = nullptr;

    double pedalValue = 0.0;
    double steerValue = 0.0;


    /**
     * Default constructor
     */
    DriverController();


    /**
     * Calculates a driver step for the pedal controller.
     * @param aDes  Desired acceleration [m/s^2]
     * @param dt    Time step size [s]
     * @param ax    Current acceleration [m/s^2]
     * @param aMin  Minimum acceleration [m/s^2]
     * @param aMax  Maximum acceleration [m/s^2]
     * @return The pedal value [-1..1]
     */
    double getPedal(double aDes, double dt, double ax, double aMin, double aMax);


    /**
     * Calculates a driver step for the steering controller based on a desired yaw rate
     * @param dPsiDes  Desired yaw rate [1/s]
     * @param dt       Time step size [s]
     * @param dPsi     Current yaw rate [1/s]
     * @return The steering angle [-2*pi..2*pi]
     */
    double getSteeringByYawRate(double dPsiDes, double dt, double dPsi);


    /**
     * Calculates a driver step for the steering controller based on a adesirec curvature
     * @param kappaDes  Desired curvature [1/m]
     * @param dt        Time step size [s]
     * @param kappa     Current yaw rate [1/s]
     * @return The steering angle [-2*pi..2*pi]
     */
    double getSteeringByCurvature(double kappaDes, double dt, double kappa);


};


#endif //SIMDRIVE_DRIVERCONTROLLER_H
