// Leap Motion streams raw frame data over Web Socket
// Johnny Five is for Arduino nodeJS interaction
var webSocket  = require('ws');
var _ = require('underscore');

var ws = new webSocket('ws://127.0.0.1:6437');

var hands, positions = [];

ws.on('message', function(data, flags) {
    hands = _.toArray(JSON.parse(data).hands);
    for (var i = 0; i < hands.length; i++) {
	positions[i] = hands[i].stabilizedPalmPosition[0];
	console.log((positions[i]+300)/600);
    }
});
