

$(function () {

    "use strict";
    console.log("connecting to server");

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
            //for (var i=0; i < json.data.length; i++) {
            //    console.log(json.data[i]);
            //}

        } else if (json.type === 'message') { // it's a single message

            // let the user write another message
            // console.log(json.data);
            update(json.data);

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


var svg = false;
var update = function(log) {

    log.size = {};
    log.size.x0 = -120;
    log.size.x1 =  120;
    log.size.y0 = -120;
    log.size.y1 =  120;

    d3.select('#time')
        .html(log.time);

    if(!svg) {

        var ratio  = (log.size.y1 - log.size.y0) / (log.size.x1 - log.size.x0);
        var width  = 1000;
        var height = width * ratio;


        svg = {};
        svg.container = d3.select("#sim-canvas")
            .append("svg")
            .attr("width", width)
            .attr("height", height);

        svg.x_scale = d3.scale.linear()
            .domain([log.size.x0, log.size.x1])
            .range([40, width - 20]);

        svg.s_scale = d3.scale.linear()
            .domain([0.0, log.size.x1 - log.size.x0])
            .range([0.0, width - 60]);

        svg.y_scale = d3.scale.linear()
            .domain([log.size.y0, log.size.y1])
            .range([height - 20, 40]);

        svg.x_axis = d3.svg.axis()
            .scale(svg.x_scale)
            .orient("bottom");

        svg.y_axis = d3.svg.axis()
            .scale(svg.y_scale)
            .orient("left");

        svg.container.append("g")
            .attr("class", "axis")
            .attr("transform", "translate(0," + (height - 20) + ")")
            .call(svg.x_axis);

        svg.container.append("g")
            .attr("class", "axis")
            .attr("transform", "translate(40,0)")
            .call(svg.y_axis);

    }


    svg.agents = svg.container
        .selectAll("rect")
        .data(log.content.agent);

    svg.agents
        .exit()
        .remove();

    svg.agents
        .enter()
        .append("rect")
        .attr("x", (d) => { return svg.x_scale(d.state.x - d.parameter.length * 0.5); })
        .attr("y", (d) => { return svg.y_scale(d.state.y + d.parameter.width * 0.5); })
        .attr("width",  (d) => { return svg.s_scale(d.parameter.length); })
        .attr("height", (d) => { return svg.s_scale(d.parameter.width); })
        .attr("rx", svg.s_scale(0.2))
        .attr("ry", svg.s_scale(0.2))
        .attr("class", "agent-box");

    svg.agents
        .attr('x', (d) => { return svg.x_scale(d.state.x - d.parameter.length * 0.5); })
        .attr('y', (d) => { return svg.y_scale(d.state.y + d.parameter.width * 0.5); })
        .attr("transform", (d) => { return "rotate(" + (-d.state.psi * 180.0 / Math.PI) + "," + svg.x_scale(d.state.x) + "," + svg.y_scale(d.state.y) + ")"; });

};