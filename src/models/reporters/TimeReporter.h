//
// Created by klimke on 27.03.2019.
//

#ifndef SIMULATION_FRAMEWORK_TIMEREPORTER_H
#define SIMULATION_FRAMEWORK_TIMEREPORTER_H

#include <core/ISynchronized.h>
#include <iostream>

class TimeReporter : public sim::ISynchronized {


    std::ostream *_stream = nullptr;


public:

    TimeReporter() = default;

    ~TimeReporter() override = default;

    void initialize(double initTime) override {

        // take std::cout if no stream is set
        if(_stream == nullptr)
            _stream = &std::cout;

    }

    void terminate(double simTime) override {}


    bool step(double simTime) override {

        // if super doesn't perform step
        if(!sim::ISynchronized::step(simTime))
            return true;

        // put sim time to stream
        (*_stream) << simTime << "s" << std::endl;

        return true;

    }


    /**
     * Sets the stream on which the time is logged
     * @param str Stream the time is to be logged in
     */
    void ostream(std::ostream &str) {

        _stream = &str;

    }

};


#endif //SIMULATION_FRAMEWORK_TIMEREPORTER_H
