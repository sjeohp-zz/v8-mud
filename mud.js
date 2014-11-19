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

var sockets = {};

function broadcast(fromuid, msg){
	for (var sockuid in sockets){
		if (sockets.hasOwnProperty(sockuid) && sockuid !== fromuid){
			sockets[sockuid].write(msg);
		}
	}
}

function write(sockuid, msg){
	if (sockets[sockuid]){
		sockets[sockuid].write(msg);
	}
}

function disconnect(sockuid){
	if (sockets[sockuid]){
		sockets[sockuid].end();
		delete sockets[sockuid];
	}
}

// setInterval(function(){
// 	mud.save();
// }, 60*.5*1000);

var server = net.createServer(function(socket) {
	socket.setKeepAlive(true, 10000);
	socket.setNoDelay(true);
	socket.uid = cuid();
	socket.playerState = String.fromCharCode(1);
	sockets[socket.uid] = socket;
	socket.write("Who are you?\n");
	socket.on('data', function(chunk){
		if (chunk.toString('utf-8').charAt(0) === String.fromCharCode(4)){
			console.log("----- EOT -----");
			if (sockets[socket.uid]){
				mud.removePlayerOnSocket(socket.uid);
				delete sockets[socket.uid];
			}
			return;
		}
		var str = socket.uid + socket.playerState + chunk.toString('utf-8').trim();
		var res = mud.process(str);
		var state = res.charAt(0);
		if (state !== String.fromCharCode(9)){
			socket.write(res.substring(1));
			socket.playerState = state;
			if (socket.playerState === String.fromCharCode(6)){
				socket.playerState = mud.process(socket.uid + socket.playerState + "").charAt(0);
			}
		}
	});
	socket.on('end', function(){

	});
	socket.on('error', function(){

	});
	socket.on('close', function(){

	});
}).listen(9000, function() {
	console.log("Listening on port 9000");
	mud.setScrypt(scrypt);
	mud.setBroadcast(broadcast);
	mud.setWrite(write);
	mud.setDisconnect(disconnect);
});