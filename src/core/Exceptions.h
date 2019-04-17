//
// Created by klimke on 14.03.2019.
//

#ifndef SIMCORE_EXCEPTIONS_H
#define SIMCORE_EXCEPTIONS_H

#include <utility>
#include <exception>
#include <string>


class ProcessException : public std::exception
{

    std::string _msg;

public:


    explicit ProcessException(const char* msg) : _msg(msg) {}

    ProcessException(const ProcessException& ex) noexcept : _msg(ex._msg) {}

    ProcessException& operator=(const ProcessException& ex) noexcept {
        _msg = ex._msg;
        return *this;
    }

    ~ProcessException() noexcept override = default;

    const char* what() const noexcept override {
        return _msg.c_str();
    }
};


class ModelNotInitialized : public ProcessException
{

public:

    explicit ModelNotInitialized(const char* msg)
        : ProcessException(msg) {}

};


#endif //SIMCORE_SIMULATIONERRORS_H
