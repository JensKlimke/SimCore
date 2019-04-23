//
// Created by klimke on 4/17/2019.
//

#ifndef SIMCORE_MODEL_H
#define SIMCORE_MODEL_H

#include "IComponent.h"
#include "IStorable.h"
#include "IParameterizable.h"
#include "ISynchronized.h"

namespace sim {

    class Model : public ISynchronized, public data::IStorable, public IParameterizable {

    public:

        Model() = default;
        ~Model() override = default;

        void terminate(double simTime) override {}

    };

}

#endif //SIMCORE_MODEL_H
