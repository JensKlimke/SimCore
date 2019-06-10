//
// Created by Jens Klimke on 2019-06-09.
//

#include "WebSocket.h"


void WebSocket::setHost(const std::string &host, const std::string &port) {

    _host = host;
    _port = port;

}


void WebSocket::connect() {

    // Look up the domain name
    auto const results = _resolver.resolve(_host, _port);

    // Make the connection on the IP address we get from a lookup
    boost::asio::connect(_ws.next_layer(), results.begin(), results.end());

    // Perform the websocket handshake
    _ws.handshake(_host, "/");

}


bool WebSocket::send(const std::string &text) {

    // Send the message
    _ws.write(boost::asio::buffer(text));

    // This buffer will hold the incoming message
    boost::beast::multi_buffer buffer;

    // Read a message into our buffer
    _ws.read(buffer);

    // The buffers() function helps print a ConstBufferSequence
    std::cout << boost::beast::buffers(buffer.data()) << std::endl;

    return true;

}


void WebSocket::close() {

    // Close the WebSocket connection
    _ws.close(websocket::close_code::normal);

}