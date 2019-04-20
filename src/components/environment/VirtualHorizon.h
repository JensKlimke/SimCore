//
// Created by Jens Klimke on 2019-04-20.
//

#ifndef SIMCORE_VIRTUALHORIZON_H
#define SIMCORE_VIRTUALHORIZON_H

#include <core/SimModel.h>


class VirtualHorizon : public ::sim::IComponent {

public:
    void initialize(double initTime) override {

    }

    bool step(double simTime) override {
        return false;
    }

    void terminate(double simTime) override {

    }

};


#endif //SIMCORE_VIRTUALHORIZON_H
