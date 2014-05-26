if (!String.prototype.format) {
  String.prototype.format = function() {
    var args = arguments;
    return this.replace(/{(\d+)}/g, function(match, number) { 
      return typeof args[number] != 'undefined' ? args[number] : match;
    });
  };
}

var net = require('net');
var cuid = require('cuid');
var scrypt = require('scrypt');
var mud = require('./build/Release/mud');
mud.setScrypt(scrypt);

var sockets = [];

function broadcast(fromuid, msg){
	sockets.forEach(function(socket){
		if (socket.uid !== fromuid){
			socket.write(msg);
		}
	});
}

var server = net.createServer(function(socket) {
	socket.setKeepAlive(true, 10000);
	socket.setNoDelay(true);
	socket.uid = cuid();
	mud.addSocket(socket);
	mud.setBroadcast(broadcast);
	sockets.push(socket);
	socket.state = String.fromCharCode(1);
	socket.write("Who are you?\n");
	socket.on('data', function(chunk){
		var str = socket.uid + socket.state + chunk.toString('utf-8').trim();
		var res = mud.process(str);
		socket.state = res.charAt(0);
		socket.write(res.substring(1));
		if (socket.state === String.fromCharCode(6)){
			socket.state = mud.process(socket.uid + socket.state + "").charAt(0);
		}
		if (str.toLowerCase() === 'quit'){
			socket.end();
		}
	});
}).listen(9000, function() {
	console.log("Listening on port 9000");
});