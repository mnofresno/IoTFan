import StaticServer from 'static-server';
import fetch from 'node-fetch';

var server = new StaticServer({
  rootPath: '.',            // required, the root of the server file tree
  port: 8187,               // required, the port to listen
  name: 'iot-server',   // optional, will set "X-Powered-by" HTTP header
  host: '0.0.0.0',       // optional, defaults to any interface
  cors: '*',                // optional, defaults to undefined
  followSymlink: true,      // optional, defaults to a 404 error
  templates: {
    index: './index.html',      // optional, defaults to 'index.html'
    notFound: '404.html'    // optional, defaults to undefined
  }
});

 
server.start(function () {
  console.log('Server listening to', server.port);
});
 
server.on('request', function (req, res) {
  if (req.path === '/poweron') {
    fetch('http://192.168.1.16:1789/iot-fan/output/on');
    res.writeHead(204, {'Content-Type': 'text/plain'});
    return res.end();
  }
  if (req.path === '/poweroff') {
    fetch('http://192.168.1.16:1789/iot-fan/output/off');
    res.writeHead(204, {'Content-Type': 'text/plain'});
    return res.end();
  }
  // req.path is the URL resource (file name) from server.rootPath
  // req.elapsedTime returns a string of the request's elapsed time
});
 
server.on('symbolicLink', function (link, file) {
  // link is the source of the reference
  // file is the link reference
  console.log('File', link, 'is a link to', file);
});
 
server.on('response', function (req, res, err, file, stat) {
  // res.status is the response status sent to the client
  // res.headers are the headers sent
  // err is any error message thrown
  // file the file being served (may be null)
  // stat the stat of the file being served (is null if file is null)
 
  // NOTE: the response has already been sent at this point
});
