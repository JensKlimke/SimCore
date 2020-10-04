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
// Created by Jens Klimke on 2019-03-14
//

#ifndef SIMCORE_EXCEPTIONS_H
#define SIMCORE_EXCEPTIONS_H

#include <utility>
#include <exception>
#include <string>


namespace sim {

    class ProcessException : public std::exception {

        std::string _msg{};

    public:


        /**
         * Constructor leaving the message empty
         */
        ProcessException() = default;


        /**
         * Constructor of an process exception
         * @param msg Message to be set
         */
        explicit ProcessException(const char *msg) : _msg(msg) {}


        /**
         * Copy constructor
         * @param ex Exception to be copied
         */
        ProcessException(const ProcessException &ex) noexcept: _msg(ex._msg) {}


        /**
         * Assignment operator
         * @param ex Exception to be copied
         * @return The copied exception
         */
        ProcessException &operator=(const ProcessException &ex) noexcept {
            _msg = ex._msg;
            return *this;
        }


        /**
         * Destructor
         */
        ~ProcessException() noexcept override = default;


        /**
         *
         * @return
         */
        [[nodiscard]] const char *what() const noexcept override {
            return _msg.c_str();
        }

    };


    /*!< Exceptions thrown when errornous setup */
    class SetupException : public ProcessException {
    public:
        SetupException() = default;
        explicit SetupException(const char *msg) : ProcessException(msg) {}
        SetupException(const SetupException &ex) noexcept: ProcessException(ex.what()) {}
        ~SetupException() override = default;
    };

}

#endif //SIMCORE_SIMULATIONERRORS_H
