//
// Created by Jens Klimke on 2019-03-24.
//

#ifndef SIMULATION_FRAMEWORK_AGENTINJECTION_H
#define SIMULATION_FRAMEWORK_AGENTINJECTION_H

#include <core/IModel.h>
#include "AgentModel.h"

class AgentInjection : public sim::IModel {

    AgentModel *_agent = nullptr;
    VehicleModel *_vehicle = nullptr;

protected:

    AgentModel::State        *_drState   = nullptr;
    AgentModel::Input        *_drInput   = nullptr;
    AgentModel::Parameters   *_drParam   = nullptr;
    AgentModel::Injection    *_drInject  = nullptr;
    VehicleModel::Input      *_vehInput  = nullptr;
    VehicleModel::State      *_vehState  = nullptr;
    VehicleModel::Parameters *_vehParam  = nullptr;


public:

    AgentInjection() = default;


    ~AgentInjection() override = default;


    /**
     * Sets the agent which will be injected
     * @param ag
     */
    void setAgent(AgentModel *ag, VehicleModel* veh) {

        _agent = ag;
        _vehicle = veh;

        // get agent state, input and parameter pointer
        _agent->getState(reinterpret_cast<void**>(&_drState));
        _agent->getInput(reinterpret_cast<void**>(&_drInput));
        _agent->getParameters(reinterpret_cast<void**>(&_drParam));
        _agent->getInjection(&_drInject);

        // get vehicle state, input and parameter pointer
        _vehicle->getState(reinterpret_cast<void**>(&_vehState));
        _vehicle->getInput(reinterpret_cast<void**>(&_vehInput));
        _vehicle->getParameters(reinterpret_cast<void**>(&_vehParam));

    }


    void initialize(double initTime) override {

        // start timer if needed in derived class
        IModel::initializeTimer(initTime);

    }


    void terminate(double simTime) override {}


};


#endif //SIMULATION_FRAMEWORK_AGENTINJECTION_H
