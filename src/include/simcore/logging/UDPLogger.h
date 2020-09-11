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

#ifndef SIMCORE_UDP_LOGGER_H
#define SIMCORE_UDP_LOGGER_H

#include <iostream>
#include <boost/asio.hpp>
#include <utility>
#include "Logger.h"

/*!< Pre-definition of UDPSocket */
namespace sim::socket {
    class UDPSocket;
}

namespace sim::logging {

    class UDPLogger : public Logger {

        sim::socket::UDPSocket *_socket = nullptr;


    public:


        /**
         * Constructor (opens connection)
         */
        UDPLogger();


        /**
         * Destructor (closes connection)
         */
        ~UDPLogger() override;


        /**
         * Sets the host and the port
         * @param host Host to be set
         * @param port Port to be set
         */
        void setHostAndPort(const std::string& host, const std::string& port);


        /**
         * Opens connection
         */
        void open() override;


        /**
         * Closes connection
         */
        void close() override;


        /**
         * Sends a message
         * @param message Message to be send
         */
        void send(const std::string &message);


        /**
         * Sends the content via udp
         * @param time The actual time
         */
        void write(double time) override;

    };

}

#endif // SIMCORE_UDP_LOGGER_H
