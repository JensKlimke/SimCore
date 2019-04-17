//
// Created by klimke on 4/17/2019.
//

#ifndef SIMCORE_SIMMODEL_H
#define SIMCORE_SIMMODEL_H

#include "IComponent.h"
#include "IStorable.h"
#include "IParameterizable.h"
#include "ISynchronized.h"

namespace sim {

    class SimModel : public ISynchronized, public IStorable, public IParameterizable {

    public:

        SimModel() = default;
        ~SimModel() override = default;

        void terminate(double simTime) override {}

    };

}

#endif //SIMCORE_SIMMODEL_H
