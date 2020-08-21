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
// Created by Jens Klimke on 2019-06-09
//

#ifndef SIMCORE_WEBSOCKET_H
#define SIMCORE_WEBSOCKET_H


#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <cstdlib>
#include <iostream>
#include <string>

using tcp = boost::asio::ip::tcp;               // from <boost/asio/ip/tcp.hpp>
namespace websocket = boost::beast::websocket;  // from <boost/beast/websocket.hpp>


class WebSocket : public std::enable_shared_from_this<WebSocket> {

    bool _connected = false;

    boost::asio::io_context ioc{};
    tcp::resolver _resolver;

    websocket::stream<tcp::socket> _ws;

    std::string _host = "127.0.0.1";
    std::string _port = "4500";
    std::string _path = "/";


public:

    WebSocket() : _resolver(ioc), _ws(ioc) {};


    ~WebSocket() = default;


    void setHost(const std::string &host, const std::string &port);


    void setPath(const std::string &path);


    bool connect();


    std::string read();


    bool send(const std::string &text);


    void close();


    bool connected() const;


};


#endif // SIMCORE_WEBSOCKET_H
