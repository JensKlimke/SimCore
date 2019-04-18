//
// Created by klimke on 4/17/2019.
//

#ifndef SIMCORE_IMODEL_H
#define SIMCORE_IMODEL_H

#include "IComponent.h"
#include "IStorable.h"
#include "IParameterizable.h"
#include "ISynchronized.h"

namespace sim {

    class IModel : public ISynchronized, public IStorable, public IParameterizable {

    public:

        IModel() = default;
        ~IModel() override = default;

        void terminate(double simTime) override {}

    };

}

#endif //SIMCORE_IMODEL_H
