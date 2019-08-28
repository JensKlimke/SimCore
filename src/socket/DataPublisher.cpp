//
// Created by Jens Klimke on 2019-06-09.
//

#include <core/functions.h>
#include <sstream>
#include <iostream>

#include "DataPublisher.h"
#include "WebSocket.h"

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