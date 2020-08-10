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
// Created by Jens Klimke.
//


#ifndef SIMCORE_TIMEREPORTER_H
#define SIMCORE_TIMEREPORTER_H

#include "../ISynchronized.h"
#include <iostream>

namespace sim::data {

    class TimeReporter : public sim::ISynchronized {


        std::ostream *_stream = nullptr;


    public:

        TimeReporter() = default;

        ~TimeReporter() override = default;

        void initialize(double initTime) override {

            // take std::cout if no stream is set
            if (_stream == nullptr)
                _stream = &std::cout;

        }

        void terminate(double simTime) override {}


        void step(double t, double dt) override {

            // put sim time to stream
            (*_stream) << "t=" << t << "s, dt=" << dt << std::endl;

        }


        /**
         * Sets the stream on which the time is logged
         * @param str Stream the time is to be logged in
         */
        void ostream(std::ostream &str) {

            _stream = &str;

        }

    };

}

#endif //SIMCORE_TIMEREPORTER_H
