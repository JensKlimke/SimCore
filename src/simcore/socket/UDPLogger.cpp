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
// Created by Jens Klimke on $date.get('yyyy-M-d').
// Contributors:
//

#include <simcore/logging/UDPLogger.h>
#include "UDPSocket.h"

namespace sim::logging {


    UDPLogger::UDPLogger() {

        // create socket
        _socket = new sim::socket::UDPSocket;

    }


    UDPLogger::~UDPLogger() {

        // delete socket
        delete _socket;

    }


    void UDPLogger::setHostAndPort(const std::string& host, const std::string& port) {

        // set host and dummy path
        this->_socket->setHost(host);
        this->_socket->setPort(port);
        this->_socket->setPath("");

    }



    void UDPLogger::open() {

        // connect
        this->_socket->connect();

    }



    void UDPLogger::close() {

        // close connection
        this->_socket->close();

    }


    void UDPLogger::send(const std::string &message) {

        // send message
        this->_socket->send(message);

    }


    void UDPLogger::write(double time) {

        // get content
        std::stringstream ss;
        Logger::writeJSONData(ss);

        // send
        send(ss.str());

    }

}


