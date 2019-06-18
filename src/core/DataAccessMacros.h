//
// Created by Jens Klimke on 2019-03-20.
//

#ifndef SIMCORE_DATAACCESSMACROS_H
#define SIMCORE_DATAACCESSMACROS_H


#define PARAM_ACCESS(struct_name, var)                                  \
bool getParameters(struct_name **params) {                              \
    try {                                                               \
        *params = &this->var;                                           \
    } catch(...) {                                                      \
        return false;                                                   \
    }                                                                   \
    return true;                                                        \
}                                                                       \


#define INPUT_ACCESS(struct_name, var)                                  \
bool getInputs(struct_name **input) {                                   \
    try {                                                               \
        *input = &this->var;                                            \
    } catch(...) {                                                      \
        return false;                                                   \
    }                                                                   \
    return true;                                                        \
}                                                                       \


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


#endif //SIMCORE_DATAACCESSMACROS_H
