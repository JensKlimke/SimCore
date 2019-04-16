//
// Created by Jens Klimke on 2019-03-20.
//

#ifndef SIMULATION_FRAMEWORK_IPLUGIN_H
#define SIMULATION_FRAMEWORK_IPLUGIN_H


namespace sim {
namespace plugin {


    bool create();
    bool initialize(double initTime);
    bool step(double simTime);
    bool terminate(double stopTime);
    bool destroy();


}} // namespace ::sim::plugin


#endif //SIMULATION_FRAMEWORK_IPLUGIN_H
