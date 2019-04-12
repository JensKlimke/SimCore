//
// Created by Jens Klimke on 2019-03-24.
//

#ifndef SIMULATION_FRAMEWORK_AGENTSTANDARDPARAMETERS_H
#define SIMULATION_FRAMEWORK_AGENTTESTPARAMETERS_H

#include <models/agent/AgentModel.h>

/**
 * Sets the standard parameters of the driver model
 * @param param
 */
void createStandardParameters(AgentModel::Parameters *param) {


    // cruise parameters
    param->cruise.a         =  1.5;
    param->cruise.aScale    =  1.0;
    param->cruise.ayMax     =  1.0;
    param->cruise.b         = -3.0;
    param->cruise.delta     =  4.0;
    param->cruise.deltaPred =  2.0;
    param->cruise.thwMax    = 15.0;
    param->cruise.thwMin    =  0.0;
    param->cruise.vMax      = 36.1;
    param->cruise.vScale    =  1.0;

    // follow parameters
    param->follow.dsStopped   = 2.0;
    param->follow.timeHeadway = 1.0;

    // these parameters have to be set in initialization
    param->vehicle.size.x = 0.0;
    param->vehicle.size.y = 0.0;
    param->vehicle.pos.x  = 0.0;
    param->vehicle.pos.y  = 0.0;

}



#endif //SIMULATION_FRAMEWORK_AGENTSTANDARDPARAMETERS_H
