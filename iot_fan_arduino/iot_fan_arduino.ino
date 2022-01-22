#include <WiFiManager.h>
//#include ".credentials.h"
#include <PubSubClient.h>

const char* mqtt_server = "192.168.1.1";
const char* topic_to_subscribe = "iot-fan/output";
const int relay = 0;

WiFiClient espClient;
WiFiManager wm;
PubSubClient client(espClient);

int i = 0;

void setup() {
    pinMode(relay, OUTPUT);
    delay(5000);
    setPowerOn();  

    WiFi.mode(WIFI_STA);
    Serial.begin(115200);
    
    bool res;
    
    res = wm.autoConnect("IoTFanESPMini","password"); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
      //if you get here you have connected to the WiFi    
      Serial.println("connected...yeey :)");
    
      wm.setConfigPortalBlocking(false);
      wm.startConfigPortal();
  
      wm.server->on("/iot-fan/output/on", [&]() {
        setPowerOn();
        wm.server->send(200, "application/json","{\"status\": \"Power on relay\"}");
      });
      wm.server->on("/iot-fan/output/off", [&]() {
        setPowerOff();
        wm.server->send(200, "application/json", "{\"status\": \"Power off relay\"}");
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
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  if (String(topic) == topic_to_subscribe) {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      setPowerOn();
    }
    else if(messageTemp == "off"){
      setPowerOff();
    }
  }
}

void setPowerOn() {
  Serial.println("on - NO relay is ON");
  digitalWrite(relay, LOW);
}

void setPowerOff() {
  Serial.println("off - NO relay is OFF");
  digitalWrite(relay, HIGH);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      Serial.print("Subscribing topic:");
      Serial.println(topic_to_subscribe);
      client.subscribe(topic_to_subscribe);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 1 seconds");
      // Wait 5 seconds before retrying
      delay(1000);
    }
  }
}
