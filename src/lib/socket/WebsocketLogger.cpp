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

#include <simcore/functions.h>
#include <simcore/logging/WebsocketLogger.h>
#include "WebSocket.h"

namespace sim::logging {

    WebsocketLogger::WebsocketLogger() {

        // create web socket
        _websocket = new sim::socket::WebSocket;

    }


    WebsocketLogger::~WebsocketLogger() {

        // delete websocket
        delete _websocket;

    }


    void WebsocketLogger::setHostAndPath(const std::string &host, const std::string &path) {

        _websocket->setHost(host);
        _websocket->setPath(path);
        _websocket->setPort("");

    }


    void WebsocketLogger::open() {

        // connect
        _websocket->connect();

    }



    void WebsocketLogger::close() {

        // close connection
        _websocket->close();

    }


    void WebsocketLogger::send(const std::string &message) {

        // send message
        _websocket->send(message);

    }


    void WebsocketLogger::write(double time) {

        // get content
        std::stringstream ss;
        Logger::writeJSONData(ss);

        // send
        send(ss.str());

    }

}