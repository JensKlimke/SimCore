// Copyright (c) 2019-2020 Jens Klimke <jens.klimke@rwth-aachen.de>
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
// Created by Jens Klimke on 2019-11-29.
//


#include "DriverController.h"


DriverController::DriverController() {

    pedal.getStates(&pState);
    steerYawRate.getStates(&sYawrateState);
    steerCurv.getStates(&sCurvState);

    pState->y        = &pedalValue;
    sYawrateState->y = &steerValue;
    sCurvState->y    = &steerValue;

}


double DriverController::getPedal(double aDes, double dt, double ax, double aMin, double aMax) {

    // get input struct
    PrimaryController::Input *pInput = nullptr;
    pedal.getInputs(&pInput);

    // limit acceleration
    aDes = std::min(aMax, std::max(aMin, aDes));

    // set input data
    pInput->value = &ax;
    pInput->target = &aDes;

    // perform step and return result
    pedal.initialize(0.0);
    pedal.step(dt);

    return pedalValue;

}


double DriverController::getSteeringByYawRate(double dPsiDes, double dt, double dPsi) {

    // get input struct
    PrimaryController::Input *sInput = nullptr;
    steerYawRate.getInputs(&sInput);

    // set input data
    sInput->value = &dPsi;
    sInput->target = &dPsiDes;

    // perform step and return result
    steerYawRate.initialize(0.0);
    steerYawRate.step(dt);

    return steerValue;

}


double DriverController::getSteeringByCurvature(double kappaDes, double dt, double kappa) {

    // get input struct
    PrimaryController::Input *sInput = nullptr;
    steerCurv.getInputs(&sInput);

    // set input data
    sInput->value = &kappa;
    sInput->target = &kappaDes;

    // perform step and return result
    steerCurv.initialize(0.0);
    steerCurv.step(dt);

    return steerValue;

}