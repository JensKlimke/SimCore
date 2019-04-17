//
// Created by Jens Klimke on 2019-03-20.
//

#ifndef SIMCORE_TIMEISUP_H
#define SIMCORE_TIMEISUP_H

#include <core/IStopCondition.h>
#include <core/IModel.h>
#include <cmath>


class TimeIsUp : public ::sim::IStopCondition, public ::sim::IModel {

public:

    struct Parameters {
        double stopTime = INFINITY;
    };


private:


    double _stopTime = INFINITY;


public:

    /**
     * Default constructor
     */
    TimeIsUp() = default;

    void initialize(double initTime) override;

    bool step(double simTime) override;

    void terminate(double simTime) override;

    void setStopTime(double stopTime);

};


#endif //SIMCORE_TIMEISUP_H
