var PORT = 3150;
var io = require('socket.io').listen(PORT);
io.set('heartbeat interval',2000);
io.set('heartbeat timeout',5000);

io.sockets.on('connection', function (socket) {

  socket.on('touch',function(data)
  {
    // 自分以外に送信
    socket.broadcast.emit("touched",data);
  });

});
