// Copyright (c) 2020 Jens Klimke.
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
// Created by Jens Klimke on 2020-09-10.
// Contributors:
//


#ifndef SIMCORE_OSTREAM_LOGGER_H
#define SIMCORE_OSTREAM_LOGGER_H

#include <ostream>
#include "Logger.h"

namespace sim::logging {

    class OstreamLogger : public Logger {

    public:

        /*!< Enum to define the log format */
        enum class LogFormat {CSV, JSON};


        /*!< Constructor */
        OstreamLogger() = default;


        /*!< Destructor */
        ~OstreamLogger() override = default;


        /**
         * Opens the logging
         */
        void open() override {

            // for json: open array bracket
            if (_format == LogFormat::JSON)
                *_ostream << (_hasContent ? ",\n\t" : "[\n");

            // reset content flag
            _hasContent = false;

        }


        /**
         * Closes the logging
         */
        void close() override {

            // close brackets
            if (_format == LogFormat::JSON)
                *_ostream << (_hasContent ? "\n" : "") << "]\n";

        }


        /**
         * Writes the values to the stream
         * @param time Actual timestamp
         */
        void write(double time) override {

            if (_format == LogFormat::JSON) {

                // save time and open object brackets
                *_ostream << (_hasContent ? ",\n\t" : "");

                // write data point to file
                Logger::writeJSONData(*_ostream);

            } else if (_format == LogFormat::CSV) {

                // write header
                if (!_hasContent)
                    writeCSVHeadline(*_ostream);

                // write data
                writeCSVData(*_ostream);

            }

            // save that data was already written
            _hasContent = true;

        }


        /**
         * Sets the log format (JSON or CSV)
         * @param format Format to be set
         */
        void setLogFormat(LogFormat format) {

            this->_format = format;

        }


        /**
         * Sets the out stream
         * @param ostream Out stream
         */
        void setOstream(std::ostream *ostream) {

            // set ostream
            this->_ostream = ostream;

        }


    protected:

        std::ostream *_ostream = nullptr;
        bool _hasContent = false;
        LogFormat _format = LogFormat::CSV;


    };

}

#endif // SIMCORE_OSTREAM_LOGGER_H
