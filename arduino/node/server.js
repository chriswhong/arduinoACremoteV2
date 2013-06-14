var http = require("http");
var url = require('url');
var fs = require('fs');
var io = require('socket.io');
var index = fs.readFileSync('index.html');
 
var server = http.createServer(function(req, res){ 
    console.log('Connection');
  res.writeHead(200, {'Content-Type': 'text/html'});
  res.end(index);
}); 
 
server.listen(8000); 
var io = require('socket.io').listen(server);

		var clients = [];

		io.sockets.on('connection', function(socket){
	
    // setInterval(function(){
    //    socket.emit('date', {'date': new Date()});
    //}, 5000);
	

	
		
	

	 socket.on('getStatus', function(data){
        //process.stdout.write(data.letter);
		io.sockets.send('getStatus');
    });
	 socket.on('togglePower', function(data){
        //process.stdout.write(data.letter);
		io.sockets.send('togglePower');
    });
	
	 socket.on('temp', function(data){
        io.sockets.emit('temp',{'temp': data.temp});
    });
	 socket.on('light', function(data){
        io.sockets.emit('light',{'light': data.light});
    });
	
	
});
