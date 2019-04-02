//
// Created by klimke on 27.03.2019.
//

#ifndef SIMULATION_FRAMEWORK_TIMEREPORTER_H
#define SIMULATION_FRAMEWORK_TIMEREPORTER_H

#include <core/ISynchronized.h>
#include <iostream>

class TimeReporter : public sim::ISynchronized {


    std::ostream &_stream = std::cout;


public:

    TimeReporter() = default;

    ~TimeReporter() override = default;

    void initialize(double initTime) override {

    }

    bool step(double simTime) override {

        // if super doesn't perform step
        if(!sim::ISynchronized::step(simTime))
            return true;

        // put sim time to stream
        _stream << simTime << "s" << std::endl;

        return true;

    }

    void terminate(double simTime) override {

    }

};


#endif //SIMULATION_FRAMEWORK_TIMEREPORTER_H
