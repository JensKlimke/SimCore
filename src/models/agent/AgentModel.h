//
// Created by Jens Klimke on 2019-03-23.
//

#ifndef SIMULATION_FRAMEWORK_AGENTMODEL_H
#define SIMULATION_FRAMEWORK_AGENTMODEL_H

#include <core/IModel.h>
#include <core/IParameterizable.h>
#include "AgentModelInterface.h"


#define STATE(key, value)                                                                           \
    _state.key = _injection.state.key != _dummyInjection.state.key ? _injection.state.key : value;  \
    _injection.state.key = value;                                                                   \



class AgentModel : public ::sim::IModel, public ::sim::IParameterizable {

public:

    struct Parameters {
        agmod::ParamCruise  cruise;
        agmod::ParamFollow  follow;
        agmod::ParamVehicle vehicle;
    };


    struct Input {
        agmod::EgoPerception ego;
        agmod::Environment   environment;
    };


    struct State {
        agmod::SubConscious subConscious;
        agmod::Conscious    conscious;
        double              aux;
    };


    struct Injection {
        Parameters parameters;
        Input      input;
        State      state;
    };


private:

    Parameters _param{};
    State      _state{};
    Input      _input{};
    Injection  _injection{};

    static const Injection _dummyInjection;


public:

    AgentModel();

    ~AgentModel() override = default;


    void initialize(double initTime) override;

    bool step(double simTime) override;

    void terminate(double simTime) override;

    // default accessors
    PARAM_ACCESS(Parameters, _param)
    INPUT_ACCESS(Input,      _input)
    STATE_ACCESS(State,      _state)


    /**
     * Overwrites the injection values of the driver model
     * @param inj Injection values
     */
    void getInjection(Injection** inj);


protected:


    void consciousStop();

    void consciousSpeed();


    void subconsciousStop(double &aRes, double &fRes);

    void subconsciousSpeed(double &aRes);


};


#endif //SIMULATION_FRAMEWORK_AGENTENMODEL_H
