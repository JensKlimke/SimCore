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
// Created by Jens Klimke on 2019-04-22
//

#ifndef SIMCORE_DATAPUBLISHER_H
#define SIMCORE_DATAPUBLISHER_H

#include "../ISynchronized.h"

// predefine WebSocket class
namespace sim::socket {
    class WebSocket;
}

namespace sim::data {

    class DataPublisher : public ::sim::ISynchronized {

        sim::socket::WebSocket *_websocket = nullptr;


    public:


        /**
         * Constructor
         */
        DataPublisher();


        /**
         * Destructor
         */
        ~DataPublisher() override;


        /**
         * Sets the host name and the port
         * @param host Host name of the server
         * @param port Port of the server
         */
        void setHost(const std::string &host, const std::string &port);


        /**
         * Sets the path of the websocket server (default is "/")
         * @param path Path of the websocket server
         */
        void setPath(const std::string &path);


        /**
         * Establishes the connection to the server
         * @param initTime Initialize time
         */
        void initialize(double initTime) override;


        /**
         * Sends the data
         * @param simTime Simulation time
         * @param deltaTime Time step size
         */
        void step(double simTime, double deltaTime) override;


        /**
         * Closes the connection to the server
         * @param simTime
         */
        void terminate(double simTime) override;


    };

}

#endif //SIMCORE_DATAPUBLISHER_H
