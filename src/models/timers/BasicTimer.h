//
// Created by Jens Klimke on 2019-03-19.
//

#ifndef SIMULATION_FRAMEWORK_BASICTIMER_H
#define SIMULATION_FRAMEWORK_BASICTIMER_H

#include <core/ITimer.h>


class BasicTimer : public ::sim::ITimer {

private:

    double _time{};
    double _stepSize{};


public:


    BasicTimer() = default;

    ~BasicTimer() override = default;

    void step() override;

    void start() override;

    void stop() override;

    double time() const override;

    void reset() override;


    /**
     * Sets the time step size of the timer
     * @param stepSize Time step size
     */
    void setTimeStepSize(double stepSize);


    /**
     * Returns the step size of the timer
     * @return Step size of the timer
     */
    double getTimeStepSize() const;


protected:

    /**
     * Sets the current time
     * @param time Time to be set
     */
    void setTime(double time);


};




#endif //SIMULATION_FRAMEWORK_STEADYTIMER_H