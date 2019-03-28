//
// Created by Jens Klimke on 2019-03-23.
//

#include "AgentModel.h"
#include "ModelCollection.h"
#include "AgentModelDummyInjection.h"
#include <iostream>


const AgentModel::Injection AgentModel::_dummyInjection = agmod::agentModelDummyInjection();

AgentModel::AgentModel() {

    // reset injection
    _injection = _dummyInjection;

}


void AgentModel::initialize(double) {

}


bool AgentModel::step(double) {

    consciousSpeed();
    subconsciousSpeed();

    // reset injection
    _injection = _dummyInjection;

    return true;

}


void AgentModel::terminate(double) {



}


void AgentModel::getInjection(AgentModel::Injection **inj) {

    *inj = &_injection;

}


void AgentModel::consciousSpeed() {


    STATE(conscious.vDes, 0.0, INFINITY);


}


void AgentModel::subconsciousSpeed() {

    // calculate reaction on local desired speed
    auto a = std::max(_param.cruise.b, std::min(_param.cruise.a,
            _param.cruise.a * ::agmod::IDMspeedReaction(_input.ego.v.x, _state.conscious.vDes, _param.cruise.delta)));

    // calculate dPsi
    double dPsi = 0.0;

    // injections
    STATE(subConscious.aDes,    a,    INFINITY);
    STATE(subConscious.dPsiDes, dPsi, INFINITY);

}
