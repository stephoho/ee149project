var _ = require('underscore');
var webSocket = require('ws'),
    ws = new webSocket('ws://127.0.0.1:6437'),
    five = require('johnny-five'),
    board = new five.Board(),
    led, frame;
var hands, positions = [];

var SerialPort = require("serialport").SerialPort
var serialPort = new SerialPort("/dev/tty-usbserial1", {
  baudrate: 9600
});

board.on('ready', function() {
    led = new five.Led(13);    
    ws.on('message', function(data, flags) {
	hands = _.toArray(JSON.parse(data).hands);
	for (var i = 0; i < hands.length; i++) {
	    positions[i] = hands[i].stabilizedPalmPosition[0];
	    led.on();
	}
	led.off();
    });
});
