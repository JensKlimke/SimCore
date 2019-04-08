//
// Created by Jens Klimke on 2019-03-24.
//

#ifndef SIMULATION_FRAMEWORK_AGENTMODELDUMMYINJECTION_H
#define SIMULATION_FRAMEWORK_AGENTMODELDUMMYINJECTION_H

#include "AgentModel.h"
#include <cmath>
#include <limits>


namespace agmod {

    /**
     * Sets the standard injection of the driver model
     * @return Injection
     */
    AgentModel::Injection agentModelDummyInjection() {

        AgentModel::Injection injection{};

        injection.parameters.cruise.a = INFINITY;
        injection.parameters.cruise.aScale = INFINITY;
        injection.parameters.cruise.ayMax = INFINITY;
        injection.parameters.cruise.b = INFINITY;
        injection.parameters.cruise.delta = INFINITY;
        injection.parameters.cruise.deltaPred = INFINITY;
        injection.parameters.cruise.thwMax = INFINITY;
        injection.parameters.cruise.thwMin = INFINITY;
        injection.parameters.cruise.vMax = INFINITY;
        injection.parameters.cruise.vScale = INFINITY;

        injection.parameters.follow.dsStopped = INFINITY;
        injection.parameters.follow.timeHeadway = INFINITY;

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

        for (auto &hor : injection.input.environment.horizon) {
            hor.ds = INFINITY;
            hor.xy.x = INFINITY;
            hor.xy.y = INFINITY;
            hor.psi = INFINITY;
            hor.kappa = INFINITY;
            hor.width.ego   = -1.0;
            hor.width.left  = -1.0;
            hor.width.right = -1.0;
        }

        for(auto &ln : injection.input.environment.lanes) {
            ln.id = -127;
            ln.width = -1.0;
            ln.route = INFINITY;
            ln.closed = INFINITY;
            ln.dir = DD_NOT_SET;
            ln.access = ACC_NOT_SET;
        }


        for(auto &sig : injection.input.environment.signals) {
            sig.id    = std::numeric_limits<uint32_t>::max();
            sig.ds    = INFINITY;
            sig.value = INFINITY;
            sig.type  = SIGNAL_NOT_SET;
        }

        injection.state.conscious.dsFollow = INFINITY;
        injection.state.conscious.dsStop = INFINITY;
        injection.state.conscious.latOffset = INFINITY;
        injection.state.conscious.stopped = INFINITY;
        injection.state.conscious.vAntic.ds = -INFINITY;
        injection.state.conscious.vAntic.value = INFINITY;
        injection.state.conscious.xyRef[0].x = INFINITY;
        injection.state.conscious.xyRef[0].y = INFINITY;
        injection.state.conscious.xyRef[1].x = INFINITY;
        injection.state.conscious.xyRef[1].y = INFINITY;
        injection.state.conscious.vDes = INFINITY;
        injection.state.conscious.vMin = INFINITY;

        injection.state.subConscious.aDes = INFINITY;
        injection.state.subConscious.dPsiDes = INFINITY;

        return injection;

    }

}


#endif //SIMULATION_FRAMEWORK_AGENTMODELDUMMYINJECTION_H
