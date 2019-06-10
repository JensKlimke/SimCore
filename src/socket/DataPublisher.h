//
// Created by Jens Klimke on 2019-04-22.
//

#ifndef SIMCORE_DATAPUBLISHER_H
#define SIMCORE_DATAPUBLISHER_H

#include <core/ISynchronized.h>
#include <components/data/DataManager.h>

class WebSocket;

namespace sim {
namespace data {

    class DataPublisher : public ::sim::ISynchronized {

        WebSocket *_websocket = nullptr;
        DataManager *_dataManager = nullptr;


    public:


        /**
         * Default constructor
         */
        DataPublisher() = default;


        /**
         * Default destructor
         */
        ~DataPublisher() override = default;


        /**
         * Sets the data manager to be send via web socket
         * @param dataManager Data manager to be send
         */
        void setDataManager(DataManager *dataManager);


        /**
         * Sets the host name and the port
         * @param host Host name of the server
         * @param port Port of the server
         */
        void setHost(const std::string &host, const std::string &port);


        /**
         * Establishes the connection to the server
         * @param initTime Initialize time
         */
        void initialize(double initTime) override;


        /**
         * Sends the data
         * @param simTime Simulation time
         */
        bool step(double simTime) override;


        /**
         * Closes the connection to the server
         * @param simTime
         */
        void terminate(double simTime) override;


    };

}}

#endif //SIMCORE_DATAPUBLISHER_H
