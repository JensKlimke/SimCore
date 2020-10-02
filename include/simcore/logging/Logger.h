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
// Created by Jens Klimke on 2019-03-16
//

#ifndef SIMCORE_LOGGER_H
#define SIMCORE_LOGGER_H

#include <vector>
#include <utility>
#include <string>
#include <cmath>
#include <ostream>
#include "../ISynchronized.h"

namespace sim::logging {


    class Logger : public ISynchronized {


        std::vector<std::pair<std::string, const double *>> _values{};


    public:


        /**
         * Creates the container
         */
        Logger() = default;


        /**
         * Closes the container
         */
        ~Logger() override = default;


        /**
         * Registers a double value to be added to the container
         * @param val Pointer to the value
         * @param key Key to be used in json
         */
        void registerValue(const std::string &key, const double *val) {

            // add pair to list
            _values.emplace_back(key, val);

        }


        /**
         * Write function
         * @param time Log time
         */
        virtual void write(double time) = 0;


        /**
         * Open function
         */
        virtual void open() = 0;


        /**
         * Close function
         */
        virtual void close() = 0;


    protected:


        /**
         * Initialize the logging, open logging
         * @param t Simulation time
         */
        void initialize(double t) override {

            // open
            open();

        }


        /**
         * A writing step, write to log
         * @param t Simulation time
         * @param dt Time step size
         */
        void step(double t, double dt) override {

            // write
            write(t);

        }


        /**
         * Finish logging, close logging
         * @param t Simulation time
         */
        void terminate(double t) override {

            // close
            close();

        }


        /**
         * Writes the headline for csv streams
         * @param out Output stream
         */
        void writeCSVHeadline(std::ostream &out) {

            // write data
            unsigned long i = 0;
            for (auto &p : _values)
                out << (i++ == 0 ? "" : ",") << "\"" << p.first << "\"";

            out << "\n";

        }


        /**
         * Writes the values to the stream as csv line
         * @param out Output stream
         */
        void writeCSVData(std::ostream &out) {

            // write data
            unsigned long i = 0;
            for (auto &p : _values) {

                // add comma
                out << (i++ == 0 ? "" : ",");

                // check for inf and nan
                if (!std::isinf(*p.second) && !std::isnan(*p.second))
                    out << *p.second;

            }

            out << "\n";

        }


        /**
         * Writes the values to the stream as JSON object
         * @param out Output stream
         */
        void writeJSONData(std::ostream &out) {

            out << "{";

            // write data
            unsigned int i = 0;
            for (auto &p : _values) {

                // stream field name
                out << (i++ == 0 ? "" : ",") << "\"" << p.first << "\":";

                // check for inf and nan
                if (std::isinf(*p.second) || std::isnan(*p.second))
                    out << "null";
                else
                    out << *p.second;

            }

            // close object brackets
            out << "}";

        }


    };


}


#endif // SIMCORE_LOGGER_H
