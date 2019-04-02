//
// Created by Jens Klimke on 2019-03-30.
//

#ifndef SIMULATION_FRAMEWORK_VALUEEXCEED_H
#define SIMULATION_FRAMEWORK_VALUEEXCEED_H

#include <core/IStopCondition.h>
#include <core/IModel.h>

template<typename T>
class ValueExceed : public ::sim::IStopCondition, public ::sim::IModel {

    const T* _value;
    T _limit;

public:

    void setValueAndLimit(const T* value, const T &limit) {

        _value = value;
        _limit = limit;

    }

    void initialize(double initTime) override {

        reset();

    }

    bool step(double simTime) override {

        if(*_value > _limit)
            end();

        return true;

    }

    void terminate(double simTime) override {

    }

};


#endif //SIMULATION_FRAMEWORK_VALUEEXCEED_H
