//
// Copyright (c) 2019-2020 Jens Klimke <jens.klimke@rwth-aachen.de>
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
// Created by Jens Klimke on 2020-03-16.
//


#ifndef SIMCORE_MODELCLASS_INTERFACE_H
#define SIMCORE_MODELCLASS_INTERFACE_H

#include <simcore/data/DataManager.h>


class ModelClass; // pre-definition

namespace _modelclass_interface {


    /** The input definition for the model */
    struct Input {
        double i; // First element
    };

    /** The state definition for the model */
    struct State {
        double s; // First element
    };

    /** The memory definition for the model */
    struct Memory {
        double m = 0.0; // First element
    };

    /** The parameters definition for the model */
    struct Parameters {
        double p; // First element
    };


    /**
     * Registers the input of the model within the data manager
     * @param model Model to be registered
     * @param input Pointer to the input
     */
    void registerInput(ModelClass *model, Input *input, sim::data::DataManager *dataManager) {

    }


    /**
     * Registers the state of the model within the data manager
     * @param model Model to be registered
     * @param state Pointer to the state
     */
    void registerState(ModelClass *model, State *state, sim::data::DataManager *dataManager) {

    }


    /**
     * Registers the memory of the model within the data manager
     * @param model Model to be registered
     * @param state Pointer to the memory
     */
    void registerMemory(ModelClass *model, Memory *memory, sim::data::DataManager *dataManager) {

    }


    /**
     * Registers the parameters of the model within the data manager
     * @param model Model to be registered
     * @param parameters Pointer to the parameters
     */
    void registerParameters(ModelClass *model, Parameters *parameters, sim::data::DataManager *dataManager) {

    }


} // namespace

#endif // SIMCORE_MODELCLASS_INTERFACE_H
