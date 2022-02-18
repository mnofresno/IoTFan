const mqtt = require('mqtt');
const express = require('express');
const app = express();
const credentials = require('./credentials.json');
const host = credentials.server;
const port = '1883';
const clientId = `mqtt_${Math.random().toString(16).slice(3)}`
const transformerPort = 1789;
const connectUrl = `mqtt://${host}:${port}`
const UNKNOWN_STATUS = 'Desconocido';
const client = mqtt.connect(connectUrl, {
  clientId,
  clean: true,
  connectTimeout: 4000,
  reconnectPeriod: 1000,
  username: credentials.username,
  password: credentials.password
})

const command_topic = 'iot-fan/output';
const status_topic_req = 'fan_status_req';
const status_topic_res = 'fan_status_res';

var currentStatus = UNKNOWN_STATUS;

var callback = null; 

var sendMessage = function(message, topic) {
  console.log(`Sending Message: ${message}`);
  client.publish(topic, message, { qos: 0, retain: false }, (error) => {
    if (error) {
      console.error(error)
    }
  });
};

var sendCommand = function(message) {
  sendMessage(message, command_topic);
};

var askStatus = function(message) {
  sendMessage(message, status_topic_req);
};

client.on('connect', () => {
  console.log('Connected')
  client.subscribe([command_topic, status_topic_res], () => {
    console.log(`Subscribe to topic '${command_topic}'`)
  });
})

client.on('message', (topic, payload) => {
  let stringPayload = payload.toString();
  console.log('Received Message:', topic, stringPayload)
  if (topic === status_topic_res) {
    currentStatus = stringPayload;
    if (callback) {
      callback();
    }
  }
})

app.get('/iot-fan/output/on', (req, res) => {
  sendCommand('on');
  res.contentType('application/json');
  res.send('{"status":"on"}')
});

app.get('/iot-fan/output/off', (req, res) => {
  sendCommand('off');
  res.contentType('application/json');
  res.send('{"status":"off"}')
});

app.get('/iot-fan/output/status', (req, res) => {
  res.contentType('application/json');
  if (currentStatus !== UNKNOWN_STATUS) {
    res.send('{"updatedStatus":"' + currentStatus + '"}')
  } else {
    callback = function() {
      res.send('{"updatedStatus":"' + currentStatus + '"}')
    };
  }
  askStatus('status');
});

app.listen(transformerPort, '0.0.0.0', () => {
  console.log(`MQTT Transformer app listening at PORT ${transformerPort}`)
});
