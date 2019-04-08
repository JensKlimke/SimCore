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

    // conscious calculation
    consciousSpeed();
    consciousStop();

    // calculate stop reaction
    double aStop, fStop;
    subconsciousStop(aStop, fStop);

    // calculate speed reaction
    double aSpeed;
    subconsciousSpeed(aSpeed);

    // calculate acceleration and yaw rate
    auto dPsi = 0.0;
    auto aRes = fStop * aSpeed + aStop;

    // injections
    STATE(subConscious.aDes, aRes)
    STATE(subConscious.dPsiDes, dPsi)

    // reset injection
    _injection = _dummyInjection;

    return true;

}


void AgentModel::terminate(double) {



}


void AgentModel::getInjection(AgentModel::Injection **inj) {

    *inj = &_injection;

}


void AgentModel::consciousStop() {


    STATE(conscious.dsStop, INFINITY)
    STATE(conscious.stopped, INFINITY)

    /*

         % get values
        ms    = mem.stop;
        env   = conc.env;
        v     = conc.ego.v.x;
        tStop = param.stop.time;
        vStop = param.stop.v;
        dist  = param.stop.ds;

        % get horizon
        ds = env.stps_ds;
        id = env.stps_id;


        % unset invalid memody
        ms.t(time < ms.t) = inf;



        %% STOP MANEUVER FINISHED

        % get stop signs which are passed
        passed = (time - ms.t) >= tStop;

        % get passed stop signs
        idP = ms.id;
        idP(~passed) = -1;

        % find ids which are passed and id which are -1
        idx = any(bsxfun(@eq, id, idP'), 1);
        idx = idx | (id == -1);

        % check if any valid stop sign
        ds(idx) = inf;

        % get next stop
        [dsStop, i] = min(ds);
        idStop = id(i);

        % check if stop is in memory
        if(~any(idStop == ms.id))

            % shift memory
            ms.id(2:end) = ms.id(1:end-1);
            ms.t(2:end)  = ms.t(1:end-1);

            % set values
            ms.id(1) = idStop;
            ms.t(1)  = inf;

        end

        % start timer if stopped
        stopping = false;
        k = find(id == ms.id(1));
        if(v < vStop && any(k) && any(id(k) ~= - 1) && all(ds(k) < (2 * dist)))

            % set stopping flag
            if(~passed(1))
                stopping = true;
            end

            % set timer
            if(isinf(ms.t(1)))
                ms.t(1) = time;
            end

        end

        % update memory
        mem.stop = ms;

     */

}


void AgentModel::consciousSpeed() {

    STATE(conscious.vDes, 20.0)
    STATE(conscious.vAntic.ds, INFINITY)
    STATE(conscious.vAntic.value, 20.0)

}


void AgentModel::subconsciousStop(double &aRes, double &fRes) {

    STATE(conscious.dsStop, INFINITY)

    // get desired distance
    auto dsDes = _param.follow.timeHeadway * _input.ego.v.x;

    // abort if stopped
    if(_state.conscious.stopped < INFINITY) {

        aRes = 0.0;
        fRes = 1.0;

        return;

    }

    // get velocity and distance
    auto v = std::max(1.0, _input.ego.v.x);
    auto ds = std::max(0.0, _state.conscious.dsStop);

    // calculate reaction
    fRes = agmod::IDMFollowReaction(aRes, ds, 0.0, v, dsDes, _param.follow.dsStopped, _param.cruise.a, _param.cruise.b);

}


void AgentModel::subconsciousSpeed(double &aRes) {


    // calculate ratio
    auto ratio = 1.0;
    auto ap = 0.0;

    // only calculate prediction, when relevant
    if (_state.conscious.vAntic.ds >= 0.0) {

        // calculate ratio
        ratio = _state.conscious.vAntic.ds / (_param.cruise.thwMax * _state.conscious.vAntic.value);
        ratio = std::pow(std::min(1.0, std::max(0.0, ratio)), _param.cruise.deltaPred);

        // calculate reaction
        ap = ::agmod::IDMSpeedReaction(_input.ego.v.x, _state.conscious.vAntic.value, _param.cruise.delta);
        ap *= ap < 0.0 ? -_param.cruise.b : _param.cruise.a;

    }

    // calculate reaction on local desired speed
    aRes = ::agmod::IDMSpeedReaction(_input.ego.v.x, _state.conscious.vDes, _param.cruise.delta);
    aRes *= aRes < 0.0 ? -_param.cruise.b : _param.cruise.a;

    // calculate acceleration
    aRes = ap * (1.0 - ratio) + ratio * aRes;

}
