//
// Created by Jens Klimke on 2019-04-01.
//

#ifndef SIMCORE_VALUEOUTOFTUBE_H
#define SIMCORE_VALUEOUTOFTUBE_H

#include <core/IStopCondition.h>
#include <core/IComponent.h>
#include <core/Exceptions.h>
#include "SignalTube.h"

class ValueOutOfTube : public SignalTube, public sim::IStopCondition, public sim::IComponent {

    IStopCondition::StopCode _mode = IStopCondition::StopCode::OBJECTIVES_MISSED;

    const double *_x = nullptr;
    const double *_y = nullptr;

public:

    typedef IStopCondition::StopCode Mode;

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
    void setValues(const double *x, const double *y, Mode mode = Mode::OBJECTIVES_MISSED) {

        _x = x;
        _y = y;
        _mode  = mode;

    }


};

#endif //SIMCORE_VALUEOUTOFTUBE_H
