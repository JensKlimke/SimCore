//
// Created by Jens Klimke on 2019-03-30.
//

#ifndef SIMULATION_FRAMEWORK_VALUEEXCEED_H
#define SIMULATION_FRAMEWORK_VALUEEXCEED_H

#include <core/IStopCondition.h>
#include <core/IModel.h>

template<typename T>
class ValueExceed : public ::sim::IStopCondition, public ::sim::IModel {

    IStopCondition::StopCode _mode = IStopCondition::StopCode::SIM_ENDED;

    const T* _value = nullptr;
    T _limit{};

public:

    typedef IStopCondition::StopCode Mode;

    void setValueAndLimit(const T* value, const T &limit, Mode mode = Mode::SIM_ENDED) {

        _value = value;
        _limit = limit;
        _mode  = mode;

    }


    void initialize(double initTime) override {

        reset();

    }


    bool step(double simTime) override {

        // the limit has been reached
        if(*_value > _limit)
            stop(_mode);

        return true;

    }


    void terminate(double simTime) override {

    }

};


#endif //SIMULATION_FRAMEWORK_VALUEEXCEED_H
