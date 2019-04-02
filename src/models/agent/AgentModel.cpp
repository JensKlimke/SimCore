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




    STATE(conscious.vDes, 20.0, INFINITY);
    STATE(conscious.vAntic.ds, INFINITY, -INFINITY);
    STATE(conscious.vAntic.value, 20.0, INFINITY);

}


void AgentModel::subconsciousSpeed() {


    // calculate ratio
    auto ratio = 1.0;
    auto ap = 0.0;

    // only calculate prediction, when relevant
    if (_state.conscious.vAntic.ds >= 0.0) {

        // calculate ratio
        ratio = _state.conscious.vAntic.ds / (_param.cruise.thwMax * _state.conscious.vAntic.value);
        ratio = std::pow(std::min(1.0, std::max(0.0, ratio)), _param.cruise.deltaPred);

        // calculate reaction
        ap = ::agmod::IDMspeedReaction(_input.ego.v.x, _state.conscious.vAntic.value, _param.cruise.delta);
        ap *= ap < 0.0 ? -_param.cruise.b : _param.cruise.a;

    }

    // calculate reaction on local desired speed
    auto a = ::agmod::IDMspeedReaction(_input.ego.v.x, _state.conscious.vDes, _param.cruise.delta);
    a *= a < 0.0 ? -_param.cruise.b : _param.cruise.a;

    // calculate acceleration
    a = ap * (1.0 - ratio) + ratio * a;

    // calculate dPsi
    double dPsi = 0.0;

    // injections
    STATE(subConscious.aDes, a, INFINITY);
    STATE(subConscious.dPsiDes, dPsi, INFINITY);

}
