#include <WiFiManager.h>
//#include ".credentials.h"
#include <PubSubClient.h>

#define MAX_LOG_BUFFER 1000
#define COMMAND_TOPIC "iot-fan/output"
#define STATUS_REQ_TOPIC "fan_status_req"
#define STATUS_RES_TOPIC "fan_status_res"

const char* mqtt_server = "192.168.1.1";
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
  Serial.println(data);
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

      wm.server->on("/log/clear", [&]() {
        logBuffer = "";
        wm.server->send(200, "text/plain charset=utf-8", "OK");
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
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    messageTemp += (char)message[i];
  }

  log(String("Message arrived on topic: " + String(topic) + " Message: " + messageTemp).c_str());

  if (String(topic) == COMMAND_TOPIC) {
    log("Changing output to ");
    if(messageTemp == "on"){
      setPowerOn();
    }
    else if(messageTemp == "off"){
      setPowerOff();
    }
  }
  if(String(topic) == STATUS_REQ_TOPIC) {
    if (messageTemp == "status") {
      sendStatus();  
    }
  }
}

void sendStatus() {
  client.publish("fan_status_res", ("status: " + currentStatus).c_str());
}

void subscribe(char* topic) {
  log(String("Subscribing topic: " + String(topic)).c_str());
  client.subscribe(topic);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    log("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      log("connected");
      // Subscribe
      subscribe(COMMAND_TOPIC);
      subscribe(STATUS_REQ_TOPIC);
    } else {
      log("failed, rc=" + client.state());
      log(" try again in 1 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
