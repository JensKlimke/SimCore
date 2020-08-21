// Copyright (c) 2020 Jens Klimke (jens.klimke@rwth-aachen.de). All rights reserved.
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
// Created by Jens Klimke on 2020-08-21.
//

#include <simcore/timers/WebSocketTimer.h>
#include "WebSocket.h"

namespace sim {

    WebSocketTimer::WebSocketTimer() {

        _websocket = new WebSocket;

    };

    WebSocketTimer::~WebSocketTimer() {

        delete _websocket;

    }

    void WebSocketTimer::start() {

        // connect and send name
        _websocket->connect();

    }

    void WebSocketTimer::stop() {

        // shut down connection
        _websocket->close();

    }

    void WebSocketTimer::receive() {

        // get data
        auto str = _websocket->read();
        auto time = std::stod(str);

        // set time
        setReferenceTime(time);

    }

}