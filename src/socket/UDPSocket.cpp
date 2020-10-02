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
// Created by Jens Klimke on 2019-06-09.
//

#include "UDPSocket.h"

namespace sim::socket {

    bool UDPSocket::connect() {

        // create service
        boost::asio::io_service io_service;

        // resolve host name
        udp::resolver resolver(io_service);
        udp::resolver::query query(udp::v4(), _host, _port);
        _endpoint = *resolver.resolve(query);

        // open connection
        _socket = new udp::socket(io_service);
        _socket->open(udp::v4());

        return true;

    }


    std::string UDPSocket::read() {

        // TODO: read buffer

        return "";

    }


    bool UDPSocket::send(const std::string &message) {

        auto data = boost::asio::buffer(message.c_str(), message.length());
        _socket->send_to(data, _endpoint);

        return true;

    }


    void UDPSocket::close() {

        // close and delete
        _socket->close();
        delete _socket;

    }


}