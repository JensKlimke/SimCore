//
// Created by Jens Klimke on 2019-06-09.
//

#ifndef BOOST_WEBSOCKET_WEBSOCKET_H
#define BOOST_WEBSOCKET_WEBSOCKET_H


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


    boost::asio::io_context ioc{};
    tcp::resolver _resolver;

    websocket::stream<tcp::socket> _ws;
    boost::beast::multi_buffer _buffer;

    std::string _host = "127.0.0.1";
    std::string _port = "4500";
    std::string _name = "sim";


public:

    WebSocket() : _resolver(ioc), _ws(ioc) {};


    ~WebSocket() = default;


    void setHost(const std::string &host, const std::string &port);


    void connect();


    bool send(const std::string &text);


    void close();

};


#endif //BOOST_WEBSOCKET_WEBSOCKET_H
