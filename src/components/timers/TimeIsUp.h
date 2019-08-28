//
// Created by Jens Klimke on 2019-03-20.
//

#ifndef SIMCORE_TIMEISUP_H
#define SIMCORE_TIMEISUP_H

#include <core/IStopCondition.h>
#include <core/IComponent.h>
#include <cmath>


#ifndef EPS_TIME
#define EPS_TIME 1e-12
#endif


class TimeIsUp : public ::sim::IStopCondition, public ::sim::IComponent {


private:


    double _stopTime = INFINITY;


public:

    /**
     * Default constructor
     */
    TimeIsUp() = default;



    void initialize(double initTime) override {

        reset();

    }


    bool step(double simTime) override {

        // set status to ended if time is reached
        if(simTime >= (_stopTime - EPS_TIME))
            end();

        return true;

    }


    void terminate(double simTime) override {}


    void setStopTime(double stopTime) {

        _stopTime = stopTime;

    }

};


#endif //SIMCORE_TIMEISUP_H
