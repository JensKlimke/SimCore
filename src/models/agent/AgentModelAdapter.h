//
// Created by Jens Klimke on 2019-03-23.
//

#ifndef SIMULATION_FRAMEWORK_AGENTMODELADAPTER_H
#define SIMULATION_FRAMEWORK_AGENTMODELADAPTER_H

#include <core/IModel.h>
#include "AgentModel.h"
#include "../VehicleModel.h"

class AgentModelAdapter : public sim::IModel {

    AgentModel *_agent = nullptr;
    VehicleModel *_vehicle = nullptr;


protected:


    /**
     * Writes the input of the agent from values of the vehicle state.
     * TODO: map connection
     */
    void writeInput() {

        // get vehicle state
        VehicleModel::State *state;
        _vehicle->getState((void**) &state);

        // get agent input
        AgentModel::Input *input;
        _agent->getInput((void**) &input);

        // copy state
        input->ego.a.x  = state->a;
        input->ego.a.y  = 0.0;
        input->ego.v.x  = state->v;
        input->ego.v.y  = 0.0;
        input->ego.dPsi = state->dPsi;
        input->ego.s    = state->s;

        // TODO: map data
        input->ego.d   = 0.0;
        input->ego.psi = 0.0;

    }


    /**
     * Writes the agents parameters from the vehicle to the agent
     */
    void writeParameters() {

        // get vehicle parameters
        VehicleModel::Parameters *param;
        _vehicle->getParameters((void**) &param);

        // get agent parameters
        AgentModel::Parameters *input;
        _agent->getInput((void**) &input);

        // copy parameters
        input->vehicle.size.x = param->size[0];
        input->vehicle.size.y = param->size[1];
        input->vehicle.pos.x  = param->driverPos[0];
        input->vehicle.pos.y  = param->driverPos[1];

    }


public:

    AgentModelAdapter() = default;

    ~AgentModelAdapter() override = default;



    void initialize(double initTime) override {

        writeParameters();
        writeInput();

    }

    bool step(double simTime) override {

        writeInput();
        return true;

    }

    void terminate(double simTime) override {

    }


    /**
     * Sets the agent and the vehicle
     * @param agent Agent model
     * @param vehicle Vehicle model
     */
    void setUnit(AgentModel *agent, VehicleModel *vehicle) {

        _agent = agent;
        _vehicle = vehicle;

    }

};


#endif //SIMULATION_FRAMEWORK_AGENTMODELADAPTER_H
