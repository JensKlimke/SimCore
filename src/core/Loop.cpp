//
// Created by Jens Klimke on 2019-03-19.
//

#include "Loop.h"
#include "IModel.h"
#include "ITimer.h"
#include "IStopCondition.h"

using namespace sim;


void Loop::setTimer(sim::ITimer *timer) {

    _timer = timer;

}


void Loop::addStopCondition(sim::IStopCondition *stop) {

    _stop_conditions.push_back(stop);

}


void Loop::addModel(sim::IModel *model) {

    _models.push_back(model);

}


void Loop::run() {

    // check status
    initialize();

    // start timer
    _timer->start();

    // iterate while stop flag is not set
    while(!_stop) {

        // iterate over models ...
        for (auto &m : _models) {

            // ... and run model step
            auto success = m->step(_timer->time());
            if (!success)
                _stop = true;

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

    // iterate over models ...
    for(auto &m : _models) {

        // ... and initialize models
        m->initialize(_timer->time());

    }

    // reset stop flag
    _stop = false;

    // set status
    _status = Status::INITIALIZED;

}


void Loop::terminate() {


    // iterate over models ...
    for (auto &m : _models) {

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