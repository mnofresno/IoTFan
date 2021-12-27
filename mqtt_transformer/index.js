const mqtt = require('mqtt');
const express = require('express');
const app = express();
const credentials = require('./credentials.json');
const host = credentials.server;
const port = '1883';
const clientId = `mqtt_${Math.random().toString(16).slice(3)}`
const transformerPort = 1789;
const connectUrl = `mqtt://${host}:${port}`
const client = mqtt.connect(connectUrl, {
  clientId,
  clean: true,
  connectTimeout: 4000,
  reconnectPeriod: 1000,
  username: credentials.username,
  password: credentials.password
})

const topic = 'iot-fan/output';

client.on('connect', () => {
  console.log('Connected')
  client.subscribe([topic], () => {
    console.log(`Subscribe to topic '${topic}'`)
  });
})

client.on('message', (topic, payload) => {
  console.log('Received Message:', topic, payload.toString())
})

var sendMessage = function(message) {
  console.log(`Sending Message: ${message}`);
  client.publish(topic, message, { qos: 0, retain: false }, (error) => {
    if (error) {
      console.error(error)
    }
  });
};

app.get('/iot-fan/output/on', (req, res) => {
  sendMessage('on');
  res.contentType('application/json');
  res.send('{"status":"on"}')
});

app.get('/iot-fan/output/off', (req, res) => {
  sendMessage('off');
  res.contentType('application/json');
  res.send('{"status":"off"}')
});

app.listen(transformerPort, () => {
  console.log(`MQTT Transformer app listening at http://localhost:${transformerPort}`)
});