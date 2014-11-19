var firmata = require("firmata");
var repl = require("repl");
var _ = require('underscore');
var webSocket = require('ws'),
    ws = new webSocket('ws://127.0.0.1:6437');
var hands, positions = [];

var serialport = "/dev/ttyACM0";

var red = 0, green = 0, blue = 0;

var board = new firmata.Board(serialport, function (error) {
    if (error) {
        console.log(error);
        return;
    }
    console.log('Connected to ' + serialport);

    board.on('string', function(data) {
        var message = '';

        for (var index = 0, length = data.length; index < length; index += 2) {
            message += String.fromCharCode(
                ((data.charCodeAt(index+1) & 0x7F) << 7) +
                (data.charCodeAt(index)   & 0x7F)
                );
        }

        if (messageHandler != null) messageHandler(message);
    });

});

rainbow(50);
ws.on('message', function(data, flags) {
    hands = _.toArray(JSON.parse(data).hands);
    for (var i = 0; i < hands.length; i++) {
	positions[0] = (hands[i].stabilizedPalmPosition[0]+300)/600;
	positions[1] = (hands[i].stabilizedPalmPosition[1]+300)/600;
	positions[2] = (hands[i].stabilizedPalmPosition[2]+300)/600;
	sendPixel(0, 255, 0, Math.floor(positions[0]*128));
    }
});



function rainbow(pixels) {
    var freq = 0.3;
    var amp = 127;
    var centre = 128; 

    var msg = "";

    for (var i = 0; i< pixels; i++) {
        setTimeout(function(n){
                var r = Math.floor(Math.sin(freq*n + 0)*amp + centre);
                var g = Math.floor(Math.sin(freq*n + 2)*amp + centre);
                var b = Math.floor(Math.sin(freq*n + 4)*amp + centre);
                
                var rgb = (r<<16) + (g<<8) + b
                sendPackedPixel(rgb, n);
        }, i*50, i);
    }
}

var sendPackedPixel = function (rgb, pos) {
    msg = "{p:" + pos + ",c:" + rgb + "}";
    sendSysExString(msg);
}


var sendPixel = function(red, green, blue, pos) {
    var send = true;

    msg = "{r:" + red +  ",g:" + green + ",b:" + blue + ",p:" + pos + "}";
    console.log(msg);
    if (send) {
        sendSysExString(msg);
    } else {
        return (msg);
    }
}


var sendSysExString = function(message) {
    var START_SYSEX = 0xF0;
    var STRING_DATA = 0x71;
    var END_SYSEX   = 0xF7;

    var buffer = new Buffer(message + '\0', 'utf8');
    var data   = [];

    data.push(START_SYSEX);
    data.push(STRING_DATA);

    for (var index = 0, length = buffer.length; index < length; index ++) {
        data.push(buffer[index] & 0x7F);
        data.push((buffer[index] >> 7) & 0x7F);
    }

    data.push(END_SYSEX);
    board.sp.write(data);
}


