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
// Created by Jens Klimke on 2020-03-06.
// Contributors:
//


#ifndef SIMCORE_UNIT_MODEL_H
#define SIMCORE_UNIT_MODEL_H

#include <algorithm>
#include <simcore/IComponent.h>
#include "Unit.h"

namespace simtraffic {

    class UnitModel : public Unit, public simcore::IComponent {

    public:


        /**
         * Constructor
         */
        UnitModel() = default;


        /**
         * Initializes the vehicle
         * @param initTime Init time
         */
        void initialize(double initTime) override {

            // reset distance
            distance = 0.0;

            // reset distance counter
            getDistance();

        }


        /**
         * Calculates a forward step of the vehicle model
         * @param time Actual time
         * @param deltaTime Time step size
         */
        void step(double time, double deltaTime) override {

            // integration
            Unit::integrate(deltaTime);

        }


    };

}


#endif // SIMCORE_UNIT_MODEL_H
