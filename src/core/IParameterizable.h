//
// Created by Jens Klimke on 2019-03-20.
//

#ifndef SIMCORE_PARAMETERIZABLE_H
#define SIMCORE_PARAMETERIZABLE_H


#define PARAM_ACCESS(struct_name, var)                                  \
bool getParameters(struct_name **params) {                              \
    try {                                                               \
        *params = &this->var;                                           \
    } catch(...) {                                                      \
        return false;                                                   \
    }                                                                   \
    return true;                                                        \
}                                                                       \

/*bool getParametersAsVoid(void **params) override {                      \
    return getParameters(reinterpret_cast<struct_name**>(params));      \
}                                                                       \*/


#define INPUT_ACCESS(struct_name, var)                                  \
bool getInputs(struct_name **input) {                                   \
    try {                                                               \
        *input = &this->var;                                            \
    } catch(...) {                                                      \
        return false;                                                   \
    }                                                                   \
    return true;                                                        \
}                                                                       \

/*bool getInputsAsVoid(void **inputs) override {                          \
    return getInputs(reinterpret_cast<struct_name**>(inputs));          \
}                                                                       \*/


#define STATE_ACCESS(struct_name, var)                                  \
bool getStates(struct_name **states) {                                  \
    try {                                                               \
        *states = &this->var;                                           \
    } catch(...) {                                                      \
        return false;                                                   \
    }                                                                   \
    return true;                                                        \
                                                                        \
}                                                                       \

/*bool getStatesAsVoid(void **states) override {                          \
    return getStates(reinterpret_cast<struct_name**>(states));          \
}                                                                       \*/


namespace sim {

class IParameterizable {

public:


    /**
     * Default constructor
     */
    IParameterizable() = default;


    /**
     * Default destructor
     */
    virtual ~IParameterizable() = default;


    /**
     * Writes the parameters of the model into the given pointer
     * @return The parameters of the model
     */
    // virtual bool getParametersAsVoid(void **params) = 0;


    /**
     * Writes the input of the model into the given pointer
     * @return The input of the model
     */
    // virtual bool getInputsAsVoid(void **inputs) = 0;


    /**
     * Writes the state of the model into the given pointer
     * @return The state of the model
     */
    // virtual bool getStatesAsVoid(void **states) = 0;


};

}; // namespace sim

#endif //SIMCORE_PARAMETERIZABLE_H
