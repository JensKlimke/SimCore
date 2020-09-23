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

        _websocket = new sim::socket::WebSocket;

        _websocket->setHost("127.0.0.1");
        _websocket->setPath("/ws");

    };

    WebSocketTimer::~WebSocketTimer() {

        delete _websocket;

    }

    void WebSocketTimer::start() {

        // set running flag
        _running = true;

        // connect and send name
        _websocket->connect();

        // The parameters to the function are put after the comma
        _thread = std::thread(WebSocketTimer::receive, this);

        // wait
        std::this_thread::sleep_for (std::chrono::milliseconds (100));

        // start super timer
        SynchronizedTimer::start();

    }

    void WebSocketTimer::stop() {

        // unset running flag
        _running = false;

        // join thread
        _thread.join();

        // shut down connection
        _websocket->close();

    }


    void WebSocketTimer::receive(WebSocketTimer *timer) {

        while(timer->_running) {

            std::cout << "Receive ... ";

            // wait 10 ms
            std::this_thread::sleep_for (std::chrono::milliseconds (10));

            // get data
            auto str = timer->_websocket->read();
            auto time = std::stod(str);

            std::cout << str << "s " << std::endl;

            // set time
            timer->setReferenceTime(time);

        }

    }

}