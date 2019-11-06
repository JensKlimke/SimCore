//
// Copyright (c) 2019 Jens Klimke <jens.klimke@rwth-aachen.de>. All rights reserved.
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
// Created by Jens Klimke on 2019-03-20
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
