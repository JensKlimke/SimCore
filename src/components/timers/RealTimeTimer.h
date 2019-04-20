//
// Created by klimke on 27.03.2019.
//

#ifndef SIMCORE_REALTIMETIMER_H
#define SIMCORE_REALTIMETIMER_H

#include "BasicTimer.h"
#include <chrono>

class RealTimeTimer : public BasicTimer {

private:

    std::chrono::system_clock::time_point _refTime;
    unsigned long _steps = 0;


public:

    RealTimeTimer() = default;

    ~RealTimeTimer() override = default;

    void step() override;

    void start() override;

    void reset() override;


};


#endif //SIMCORE_REALTIMETIMER_H
