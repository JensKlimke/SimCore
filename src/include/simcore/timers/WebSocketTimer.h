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
// Created by Jens Klimke on 2020-08-21.
//

#ifndef SIMCORE_WEBSOCKET_TIMER_H
#define SIMCORE_WEBSOCKET_TIMER_H

#include <thread>
#include "SynchronizedTimer.h"

namespace sim::socket {
    class WebSocket;
}

namespace sim {

    class WebSocketTimer : public SynchronizedTimer {

        sim::socket::WebSocket *_websocket = nullptr;

        std::thread _thread;
        bool _running{};


    public:

        /**
         * Constructor
         */
        WebSocketTimer();


        /**
         * Destructor
         */
        ~WebSocketTimer() override;


        /**
         * Start the timer
         */
        void start() override;


        /**
         * Stops the timer
         */
        void stop() override;


        /**
         * Receives the time from the server
         */
        static void receive(WebSocketTimer *timer);

    };

}


#endif // SIMCORE_WEBSOCKET_TIMER_H