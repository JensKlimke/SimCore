//
// Created by Jens Klimke on 2019-03-19.
//

#ifndef SIMCORE_BASICTIMER_H
#define SIMCORE_BASICTIMER_H

#include <core/ITimer.h>


class BasicTimer : public ::sim::ITimer {

private:

    double _time{};
    double _stepSize{};


public:


    /**
     * Default constructor
     */
    BasicTimer() = default;


    ~BasicTimer() override = default;


    void step() override {

        _time += _stepSize;

    }


    void start() override {}

    void stop() override {}


    double time() const override {

        return _time;

    }


    void reset() override {

        _time = 0.0;

    }


    /**
     * Sets the time step size of the timer
     * @param stepSize Time step size
     */
    void setTimeStepSize(double stepSize) {

        _stepSize = stepSize;

    }


    /**
     * Returns the step size of the timer
     * @return Step size of the timer
     */
    double getTimeStepSize() const {

        return _stepSize;

    }


protected:

    /**
     * Sets the current time
     * @param time Time to be set
     */
    void setTime(double time) {

        _time = time;

    }


};




#endif //SIMCORE_STEADYTIMER_H
