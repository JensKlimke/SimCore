// Copyright (c) 2021 Jens Klimke.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// Created by Jens Klimke on 2021-03-17.
// Contributors:
//


#ifndef SIMCORE_GENERICCOMPONENT_H
#define SIMCORE_GENERICCOMPONENT_H

#include "IComponent.h"

namespace sim {

    class GenericComponent : public IComponent {

    public:

        // step functions to be added manually
        std::function<void(double)> stepFnc;
        std::function<void(double)> initFnc;
        std::function<void(double)> termFnc;


        void initialize(double initTime) override {

            if(initFnc)
                initFnc(initTime);

        }

        void terminate(double simTime) override {

            if(termFnc)
                termFnc(simTime);

        }

        void step(double simTime) override {

            // step function
            if(stepFnc)
                stepFnc(simTime);

        }

    };

}

#endif //SIMCORE_GENERICCOMPONENT_H
