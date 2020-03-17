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

#include <WebSocket.h>


void WebSocket::setHost(const std::string &host, const std::string &port) {

    _host = host;
    _port = port;

}


void WebSocket::setPath(const std::string &path) {

    _path = path;

}


bool WebSocket::connect() {

    // Look up the domain name
    auto const results = _resolver.resolve(_host, _port);

    try {

        // Make the connection on the IP address we get from a lookup
        boost::asio::connect(_ws.next_layer(), results.begin(), results.end());

        // Perform the websocket handshake
        _ws.handshake(_host, _path);

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