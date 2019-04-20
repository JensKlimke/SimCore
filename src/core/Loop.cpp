//
// Created by Jens Klimke on 2019-03-19.
//

#include "Loop.h"
#include "IComponent.h"
#include "ITimer.h"
#include "IStopCondition.h"
#include <exception>

using namespace sim;


void Loop::setTimer(sim::ITimer *timer) {

    _timer = timer;

}


void Loop::addStopCondition(sim::IStopCondition *stop) {

    _stop_conditions.push_back(stop);

}


void Loop::addModel(sim::IComponent *model) {

    _components.push_back(model);

}


void Loop::run() {

    // check status
    initialize();

    // start timer
    _timer->start();

    // iterate while stop flag is not set
    while(!_stop) {

        // iterate over components ...
        for (auto &m : _components) {

            // ... and run model step
            m->step(_timer->time());

        }

        // iterate over stop conditions ...
        for(auto &sc : _stop_conditions) {

            // ... and check status
            if (sc->hasStopped())
                _stop = true;

        }

        // time step
        if(!_stop)
            _timer->step();

    }

    // stop timer
    _timer->stop();

    // if loop ended, terminate regularly
    terminate();

}


void Loop::stop() {

    // check state
    if(_status != Status::RUNNING)
        throw ProcessException("Simulation is not running.");

    // set stop flag
    _stop = true;

}


void Loop::initialize() {

    // check status
    if(_status != Status::STOPPED)
        throw ProcessException("Simulation must be stopped to be initialized.");

    // reset timer
    _timer->reset();

    // iterate over stop conditions ...
    for(auto &sc : _stop_conditions) {

        // ... and reset
        sc->reset();

    }

    // iterate over components ...
    for(auto &m : _components) {

        // ... and initialize models
        m->initialize(_timer->time());

    }

    // reset stop flag
    _stop = false;

    // set status
    _status = Status::INITIALIZED;

}


void Loop::terminate() {


    // iterate over components ...
    for (auto &m : _components) {

        // ... and terminate
        m->terminate(_timer->time());

    }

    // set status
    _status = Status::STOPPED;

}


Loop::Status Loop::getStatus() const {

    // status
    return _status;

}