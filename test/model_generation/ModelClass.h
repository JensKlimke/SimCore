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


#ifndef SIMCORE_MODELCLASS_H
#define SIMCORE_MODELCLASS_H

#include <simcore/Model.h>
#include "ModelClass_Interface.h"

/**
 * @brief A sample model class
 * The model class implements some basic functions
 */
class ModelClass : public sim::Model {

public:

    /** Internal representation class for input data */
    typedef _modelclass_interface::Input Input;

    /** Internal representation class for state data */
    typedef _modelclass_interface::State State;

    /** Internal representation class for memory data */
    typedef _modelclass_interface::Memory Memory;

    /** Internal representation class for parameters data */
    typedef _modelclass_interface::Parameters Param;


    /** Default constructor */
    ModelClass() = default;


    /** Default destructor */
    ~ModelClass() override = default;


    /**
     * Method to create the model instance
     * @return Flag indicating the success of creation
     */
    virtual bool create() {}


    /**
     * Method to register the model instance interface to a data manager
     * @param dataManager The data manager, the instance shall be registered in
     * @param name The name of the instance
     * @return Flag indicating the success of registering
     */
    virtual bool registerInterface(sim::data::DataManager *dataManager, const std::string &name) {

        _modelclass_interface::registerInput(this, &input, dataManager);      // registering input
        _modelclass_interface::registerState(this, &state, dataManager);      // registering state
        _modelclass_interface::registerMemory(this, &memory, dataManager);    // registering memory
        _modelclass_interface::registerParameters(this, &param, dataManager); // registering parameters

    }


    /**
     * Method before initializing the model instance, TODO: describe purpose
     * @param initTime Initialization time
     * @return Flag indicating the success of pre-initialization
     */
    bool preInit(double initTime) override;


    /**
     * Method to initialize the model instance
     * @param initTime Initialization time
     * @return Flag indicating the success of initialization
     */
    bool initialize(double initTime) override;


    /**
     * Method to be executed before the step is executed
     * @param simTime Simulation time
     * @oaram deltaTime Time step size
     * @return Flag indicating the success of pre-step
     */
    bool preStep(double simTime, double deltaTime) override;


    /**
     * Method to be executed the step is executed
     * @param simTime Simulation time
     * @oaram deltaTime Time step size
     * @return Flag indicating the success of pre-step
     */
    bool step(double simTime, double deltaTime) override;


    /**
     * Method to be executed after the step is executed
     * @param simTime Simulation time
     * @oaram deltaTime Time step size
     * @return Flag indicating the success of pre-step
     */
    bool postStep(double simTime, double deltaTime) override;


    /**
     * Method to terminate the execution
     * @param simTime Simulation time
     * @return Flag indicating the success of pre-step
     */
    bool terminate(double simTime) override;


    /**
     * Method to destroy the instance
     * @return Flag indicating the success of pre-step
     */
    bool destroy();


protected:

    Input input{};    // input of the instance
    State state{};    // state of the instance
    Memory memory{};  // memory of the instance
    Param param{};    // parameters of the instance


};


#endif // SIMCORE_MODELCLASS_H
