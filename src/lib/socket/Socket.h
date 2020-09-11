// Copyright (c) 2020 Jens Klimke.
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
// Created by Jens Klimke on $date.get('yyyy-M-d').
// Contributors:
//

#ifndef SIMCORE_SOCKET_H
#define SIMCORE_SOCKET_H

#include <string>

namespace sim::socket {

    class Socket {

    protected:

        std::string _host = "127.0.0.1";
        std::string _port = "4500";
        std::string _path = "/";

        bool _connected = false;

    public:


        /**
         * Sets the host
         * @param host Host name of the server
         */
        void setHost(const std::string &host) {

            _host = host;

        }


        /**
         * Sets the port of
         * @param port Port of the server
         */
        void setPort(const std::string &port) {

            _port = port;

        }


        /**
         * Sets the path
         * @param path Path of the server
         */
        void setPath(const std::string &path) {

            _path = path;

        }


        /**
         * Returns flag indication whether a connection is established
         * @return Connection flag
         */
        [[nodiscard]] virtual bool connected() const {

            return _connected;

        };


        /**
         * Connects the socket
         * @return Success flag
         */
        virtual bool connect() = 0;


        /**
         * Reads a string from the buffer
         * @return String to be read
         */
        virtual std::string read() = 0;


        /**
         * Sends the string to the server
         * @param text Text to be sent
         * @return Success flag
         */
        virtual bool send(const std::string &text) = 0;


        /**
         * Closes the connection
         */
        virtual void close() = 0;


    };

}

#endif //SIMCORE_SOCKET_H
