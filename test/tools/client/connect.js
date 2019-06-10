$(function () {

    "use strict";

    // if user is running mozilla then use it's built-in WebSocket
    window.WebSocket = window.WebSocket || window.MozWebSocket;

    // if browser doesn't support WebSocket, just show
    // some notification and exit
    if (!window.WebSocket) {

        alert('Sorry, but your browser doesn\'t support WebSocket.');
        return;

    }

    // open connection
    var connection = new WebSocket('ws://127.0.0.1:4500');
    connection.onopen = function () {

        console.log('Connected ...');

    };

    connection.onerror = function (error) {

        alert('Error while connecting (' + error + ') ...')

    };

    // most important part - incoming messages
    connection.onmessage = function (message) {

        // try to parse JSON message. Because we know that the server
        // always returns JSON this should work without any problem but
        // we should make sure that the massage is not chunked or
        // otherwise damaged.
        try {

            var json = JSON.parse(message.data);

        } catch (e) {

            console.log('Invalid JSON: ', message.data);
            return;

        }

        // NOTE: if you're not sure about the JSON structure
        // check the server source code above
        // first response from the server with user's color
        if (json.type === 'status') {

            // log
            console.log('Client accepted');

        } else if (json.type === 'history') { // entire message history

            // insert every single message to the chat window
            for (var i=0; i < json.data.length; i++) {
                console.log(json.data[i]);
            }

        } else if (json.type === 'message') { // it's a single message

            // let the user write another message
            console.log(json.data);

        } else {

            console.log('Hmm..., I\'ve never seen JSON like this:', json);

        }
    };

    /**
     * This method is optional. If the server wasn't able to
     * respond to the in 3 seconds then show some error message
     * to notify the user that something is wrong.
     */
    setInterval(function() {

        if (connection.readyState !== 1)
            $('time').text('Unable to communicate with the WebSocket server.');

    }, 3000);


    setTimeout(function() {

        if (connection.readyState === 1)
            connection.send('web');

    }, 3000);

});