var express = require('express');
const https = require('http');

var app = express();
app.use(express.static(__dirname));
app.listen(process.env.PORT || 8187);

// import fetch from 'node-fetch';

// var transformerBaseUrl = 'http://192.168.1.16:1789';
var transformerBaseUrl = 'http://localhost:1789';
 
app.get('/poweron', function (req, res) {
    https.get(transformerBaseUrl + '/iot-fan/output/on');
    return res.writeHead(204, {'Content-Type': 'text/plain'});
});

app.get('/poweroff', function (req, res) {
    https.get(transformerBaseUrl + '/iot-fan/output/off');
    return res.writeHead(204, {'Content-Type': 'text/plain'});
});

app.get('/status', function (req, res) {
  https.get(transformerBaseUrl + '/iot-fan/output/status', function (statusResponse) {
    var body = '';
    statusResponse.on('data', function (chunk) {
      body += chunk;
    });
    statusResponse.on('end', function () {
      res.contentType('application/json');
      return res.send(200, body)
    });
  }).end();
});