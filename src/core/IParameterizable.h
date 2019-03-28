//
// Created by Jens Klimke on 2019-03-20.
//

#ifndef SIMULATION_FRAMEWORK_PARAMETERIZABLE_H
#define SIMULATION_FRAMEWORK_PARAMETERIZABLE_H


#define PARAM_ACCESS(type, var)                                         \
bool getParameters(void **param) override {                             \
    try {                                                               \
        *param = &var;                                                  \
    } catch(...) {                                                      \
        return false;                                                   \
    }                                                                   \
    return true;                                                        \
}                                                                       \


#define INPUT_ACCESS(type, var)                                         \
bool getInput(void **input) override {                                  \
    try {                                                               \
        *input = &var;                                                  \
    } catch(...) {                                                      \
        return false;                                                   \
    }                                                                   \
    return true;                                                        \
}                                                                       \


#define STATE_ACCESS(type, var)                                         \
bool getState(void **state) override {                                  \
    try {                                                               \
        *state = &var;                                                  \
    } catch(...) {                                                      \
        return false;                                                   \
    }                                                                   \
    return true;                                                        \
}                                                                       \



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
    virtual bool getParameters(void **param) = 0;


    /**
     * Writes the input of the model into the given pointer
     * @return The input of the model
     */
    virtual bool getInput(void **input) = 0;


    /**
     * Writes the state of the model into the given pointer
     * @return The state of the model
     */
    virtual bool getState(void **state) = 0;


};

}; // namespace sim

#endif //SIMULATION_FRAMEWORK_PARAMETERIZABLE_H
