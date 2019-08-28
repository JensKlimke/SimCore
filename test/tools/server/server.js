"use strict";

// Optional. You will see this name in eg. 'ps' or 'top' command
process.title = 'node-chat';

var webSocketsServerPort = 4500;
var webSocketServer = require('websocket').server;
var http = require('http');


/**
 * Global variables
 */
// latest 100 messages
var history = [ ];
var clients = [ ];

/**
 * Helper function for escaping input strings
 */
function htmlEntities(str) {
    return String(str)
        .replace(/&/g, '&amp;').replace(/</g, '&lt;')
        .replace(/>/g, '&gt;').replace(/"/g, '&quot;');
}


/**
 * HTTP server
 */
var server = http.createServer(function(request, response) {
    // Not important for us. We're writing WebSocket server,
    // not HTTP server
});

server.listen(webSocketsServerPort, function() {
    console.log((new Date()) + " Server is listening on port "
        + webSocketsServerPort);
});

/**
 * WebSocket server
 */
var wsServer = new webSocketServer({
    // WebSocket server is tied to a HTTP server. WebSocket
    // request is just an enhanced HTTP request. For more info
    // http://tools.ietf.org/html/rfc6455#page-6
    httpServer: server
});

// This callback function is called every time someone
// tries to connect to the WebSocket server
wsServer.on('request', function(request) {

    console.log((new Date()) + ' Connection from origin ' + request.origin + '.');

    // accept connection - you should check 'request.origin' to
    // make sure that client is connecting from your website
    // (http://en.wikipedia.org/wiki/Same_origin_policy)
    var connection = request.accept(null, request.origin);

    // we need to know client index to remove them on 'close' event
    var index = clients.push(connection) - 1;
    var userName = false;

    console.log((new Date()) + ' Connection accepted.');

    // send back chat history
    if (history.length > 0)
        connection.sendUTF(JSON.stringify({ type: 'history', data: history} ));

    // user sent some message
    connection.on('message', function(message) {

        if (message.type === 'utf8') { // accept only text

            // first message sent by user is their name
            if (userName === false) {

                // remember user name
                userName = htmlEntities(message.utf8Data);

                // check if user name is sim and clear history if so
                if(userName === 'sim')
                    history = [];

                // send accepted code
                connection.sendUTF(JSON.stringify({ type:'status', data: 'accepted' }));

                // log
                console.log((new Date()) + ' User is known as: ' + userName);

            } else {

                // we want to keep history of all sent messages
                var obj = {
                    time: (new Date()).getTime(),
                    content: JSON.parse(message.utf8Data),
                    author: userName,
                };

                // add message to history
                history.push(obj);
                history = history.slice(-100);

                // broadcast message to all connected clients
                var json = JSON.stringify({ type: 'message', data: obj });
                for (var i=0; i < clients.length; i++)
                    clients[i].sendUTF(json);

            }
        }
    });

    // user disconnected
    connection.on('close', function(connection) {

        if (userName !== false) {

            console.log((new Date()) + " Peer " + connection.remoteAddress + " disconnected.");

            // remove user from the list of connected clients
            clients.splice(index, 1);

        }

    });
});