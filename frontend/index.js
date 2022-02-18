var transformerBaseUrl = 'http://localhost:1789';

var express = require('express');
const https = require('http');

var app = express();
app.use(express.static(__dirname));
app.listen(process.env.PORT || 8187);

var makeRequest = function (url, successCallback) {
  var request = https.get(transformerBaseUrl + url, function (statusResponse) {
    var body = '';
    statusResponse.on('data', function (chunk) {
      body += chunk;
    });
    statusResponse.on('end', function () { successCallback(body); });
  });
  request.on('error', function (info) { console.log(info); });
  request.end();
};

app.get('/poweron', function (req, res) {
    https.get(transformerBaseUrl + '/iot-fan/output/on');
    return res.writeHead(204, {'Content-Type': 'text/plain'});
});

app.get('/poweroff', function (req, res) {
    https.get(transformerBaseUrl + '/iot-fan/output/off');
    return res.writeHead(204, {'Content-Type': 'text/plain'});
});

app.get('/status', function (req, res) {
  makeRequest('/iot-fan/output/status', function (body) {
    res.contentType('application/json');
    return res.status(200).json(body);
  });
});