//
// Created by klimke on 27.03.2019.
//

#ifndef SIMULATION_FRAMEWORK_REALTIMETIMER_H
#define SIMULATION_FRAMEWORK_REALTIMETIMER_H

#include "BasicTimer.h"
#include <chrono>

class RealTimeTimer : public BasicTimer {

private:

    std::chrono::system_clock::time_point _refTime;
    unsigned long _steps;


public:

    RealTimeTimer() = default;

    ~RealTimeTimer() override = default;

    void step() override;

    void start() override;

    void reset() override;


};


#endif //SIMULATION_FRAMEWORK_REALTIMETIMER_H
