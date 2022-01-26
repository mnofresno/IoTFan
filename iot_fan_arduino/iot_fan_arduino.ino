#include <WiFiManager.h>
//#include ".credentials.h"
#include <PubSubClient.h>

#define MAX_LOG_BUFFER 1000

const char* mqtt_server = "192.168.1.1";
const char* topic_to_subscribe = "iot-fan/output";
const int relay = 0;

String currentStatus = "";
WiFiClient espClient;
WiFiManager wm;
PubSubClient client(espClient);

String logBuffer = "";

void log(const char* data = "") {
  if (logBuffer.length() > MAX_LOG_BUFFER) {
    logBuffer = "";
  }
  logBuffer += String(millis()) + "> " + String(data) + "\n" + logBuffer;
}

void setup() {
    log("Starting CPU...");
    pinMode(relay, OUTPUT);    
    setPowerOff();
    delay(5000);
    setPowerOn();
    
    WiFi.mode(WIFI_STA);
    Serial.begin(115200);
    
    bool res;
    
    res = wm.autoConnect("IoTFanESPMini","password"); // password protected ap

    if(!res) {
        log("Failed to connect");
        // ESP.restart();
    } 
    else {
      //if you get here you have connected to the WiFi    
      log("connected...yeey :)");
    
      wm.setConfigPortalBlocking(false);
      wm.startConfigPortal();

      wm.server->on("/log", [&]() {
        wm.server->send(200, "text/plain charset=utf-8", logBuffer);
      });

      client.setServer(mqtt_server, 1883);
      client.setCallback(mqtt_callback);
    }
}

void loop() {
    wm.process();
    if (!client.connected()) {
      reconnect();
    }
    client.loop();
}

void mqtt_callback(char* topic, byte* message, unsigned int length) {
  log("Message arrived on topic: ");
  log(topic);
  log(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    log((char)message[i]);
    messageTemp += (char)message[i];
  }
  log();

  if (String(topic) == topic_to_subscribe) {
    log("Changing output to ");
    if(messageTemp == "on"){
      setPowerOn();
    }
    else if(messageTemp == "off"){
      setPowerOff();
    }
  }
  if(String(topic) == "fan_status_req") {
    if (messageTemp == "status") {
      sendStatus();  
    }
  }
}

void sendStatus() {
  client.publish("fan_status_res", ("status: " + currentStatus).c_str());
}

void setPowerOn() {
  log("on - NO relay is ON");
  digitalWrite(relay, LOW);
  currentStatus = "on";
  sendStatus();
}

void setPowerOff() {
  log("off - NO relay is OFF");
  digitalWrite(relay, HIGH);
  currentStatus = "off";
  sendStatus();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    log("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      log("connected");
      // Subscribe
      log("Subscribing topic:");
      log(topic_to_subscribe);
      client.subscribe(topic_to_subscribe);
    } else {
      log("failed, rc=");
      log(client.state());
      log(" try again in 1 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
