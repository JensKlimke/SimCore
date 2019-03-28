//
// Created by Jens Klimke on 2019-03-20.
//

#ifndef SIMULATION_FRAMEWORK_IPLUGIN_H
#define SIMULATION_FRAMEWORK_IPLUGIN_H


#include "IParameterizable.h"

namespace sim {

    class IPlugin : public IParameterizable {

    public:

        IPlugin() = default;

        virtual ~IPlugin() = default;

    };

}; // namespace ::sim


#endif //SIMULATION_FRAMEWORK_IPLUGIN_H
