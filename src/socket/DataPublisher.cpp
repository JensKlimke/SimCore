//
// Copyright (c) 2019 Jens Klimke <jens.klimke@rwth-aachen.de>. All rights reserved.
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

#include <sstream>
#include <iostream>

#include <simcore/functions.h>
#include <simcore/socket/DataPublisher.h>
#include <simcore/socket/WebSocket.h>

namespace sim {
namespace data {


    void DataPublisher::setDataManager(sim::data::DataManager *dataManager) {

        _dataManager = dataManager;

    }


    void DataPublisher::setHost(const std::string &host, const std::string &port) {

        _websocket->setHost(host, port);

    }


    void DataPublisher::initialize(double initTime) {

        ::sim::ISynchronized::initialize(initTime);

        // create web socket
        _websocket = new WebSocket;

        // connect and send name
        _websocket->connect();
        _websocket->send("sim");

        // TODO: wait for acceptance

    }


    bool DataPublisher::step(double simTime) {

        if (::sim::ISynchronized::step(simTime)) {

            // serialize data ...
            std::stringstream ss;
            _dataManager->streamTo(ss);

            // .. and send
            _websocket->send(ss.str());

        }

        return true;

    }


    void DataPublisher::terminate(double simTime) {

        // close and delete web socket
        _websocket->close();
        delete _websocket;

    }

}}