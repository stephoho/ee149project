// Leap Motion streams raw frame data over Web Socket
// Johnny Five is for Arduino nodeJS interaction
var webSocket  = require('ws');
var johnnyFive = require('johnny-five');

var ws    = new webSocket('ws://127.0.0.1:6437');
var board = new johnnyFive.Board();

var hands, positions;

board.on('ready', function() {
    ws.on('message', function(data, flags) {
	hands = JSON.parse(data).hands;
	for (var i = 0; i &lt; hands.length; i++) {
	    stabilized = hand.stabilizedPalmPosition;
	    
	}
    });
});
