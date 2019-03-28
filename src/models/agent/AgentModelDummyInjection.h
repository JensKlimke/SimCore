//
// Created by Jens Klimke on 2019-03-24.
//

#ifndef SIMULATION_FRAMEWORK_AGENTMODELDUMMYINJECTION_H
#define SIMULATION_FRAMEWORK_AGENTMODELDUMMYINJECTION_H

#include "AgentModel.h"
#include <cmath>


namespace agmod {

    /**
     * Sets the standard injection of the driver model
     * @return Injection
     */
    AgentModel::Injection agentModelDummyInjection() {

        AgentModel::Injection injection{};

        injection.parameters.cruise.a = INFINITY;
        injection.parameters.cruise.b = INFINITY;
        injection.parameters.cruise.vScale = INFINITY;
        injection.parameters.cruise.thwMin = INFINITY;
        injection.parameters.cruise.thwMax = INFINITY;
        injection.parameters.cruise.delta = INFINITY;
        injection.parameters.cruise.deltaPred = INFINITY;
        injection.parameters.cruise.aScale = INFINITY;

        injection.parameters.vehicle.pos.x = INFINITY;
        injection.parameters.vehicle.pos.y = INFINITY;
        injection.parameters.vehicle.size.x = INFINITY;
        injection.parameters.vehicle.size.y = INFINITY;

        injection.input.ego.a.x = INFINITY;
        injection.input.ego.a.y = INFINITY;
        injection.input.ego.v.x = INFINITY;
        injection.input.ego.v.y = INFINITY;
        injection.input.ego.s = INFINITY;
        injection.input.ego.d = INFINITY;
        injection.input.ego.dPsi = INFINITY;
        injection.input.ego.psi = INFINITY;

        injection.state.conscious.vDes = INFINITY;
        injection.state.conscious.dsStop = INFINITY;
        injection.state.conscious.latOffset = INFINITY;
        injection.state.conscious.vMin = INFINITY;
        injection.state.conscious.vAntic.ds = -INFINITY;
        injection.state.conscious.vAntic.value = INFINITY;
        injection.state.conscious.xyRef[0].x = INFINITY;
        injection.state.conscious.xyRef[0].y = INFINITY;
        injection.state.conscious.xyRef[1].x = INFINITY;
        injection.state.conscious.xyRef[1].y = INFINITY;

        injection.state.subConscious.aDes = INFINITY;
        injection.state.subConscious.dPsiDes = INFINITY;

        return injection;

    }

}


#endif //SIMULATION_FRAMEWORK_AGENTMODELDUMMYINJECTION_H
