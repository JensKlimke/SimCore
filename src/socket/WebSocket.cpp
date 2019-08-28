//
// Created by Jens Klimke on 2019-06-09.
//

#include "WebSocket.h"


void WebSocket::setHost(const std::string &host, const std::string &port) {

    _host = host;
    _port = port;

}


bool WebSocket::connect() {

    // Look up the domain name
    auto const results = _resolver.resolve(_host, _port);

    try {

        // Make the connection on the IP address we get from a lookup
        boost::asio::connect(_ws.next_layer(), results.begin(), results.end());

        // Perform the websocket handshake
        _ws.handshake(_host, "/");

    } catch(...) {

        return false;

    }

    // set connected flag
    _connected = true;

    return true;

}


bool WebSocket::send(const std::string &text) {

    if(!_connected)
        return false;

    // Send the message
    _ws.write(boost::asio::buffer(text));

    return true;

}


void WebSocket::close() {

    // Close the WebSocket connection
    if(_connected)
        _ws.close(websocket::close_code::normal);

}


bool WebSocket::connected() const {

    return _connected;

}