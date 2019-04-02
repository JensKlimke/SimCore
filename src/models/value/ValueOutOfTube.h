//
// Created by Jens Klimke on 2019-04-01.
//

#ifndef SIMULATION_FRAMEWORK_VALUEOUTOFTUBE_H
#define SIMULATION_FRAMEWORK_VALUEOUTOFTUBE_H

#include <core/IStopCondition.h>
#include <core/IModel.h>
#include <core/Exceptions.h>
#include "SignalTube.h"

class ValueOutOfTube : public SignalTube, public sim::IStopCondition, public sim::IModel {

    const double *_x = nullptr;
    const double *_y = nullptr;

public:

    ValueOutOfTube() = default;

    void initialize(double initTime) override {

        if(!isSet())
            throw ModelNotInitialized("Band is not initialized.");

    }

    bool step(double simTime) override {

        if(!in(*_x, *_y))
            failed();

        return true;

    }

    void terminate(double simTime) override {

    }


    /**
     * Sets the values to be checked
     * @param x x-value
     * @param y y-value
     */
    void setValues(const double *x, const double *y) {

        _x = x;
        _y = y;

    }


};

#endif //SIMULATION_FRAMEWORK_VALUEOUTOFTUBE_H
