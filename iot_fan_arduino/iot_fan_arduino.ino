#include <WiFiManager.h>
//#include ".credentials.h"
#include <PubSubClient.h>
#include <WiFi.h>

const char* mqtt_server = "mqtt-broker.lan";
const char* topic_to_subscribe = "iot-fan/output";

const int relay = 26;

WiFiClient espClient;
WiFiManager wm;
PubSubClient client(espClient);


void setup() {
    pinMode(relay, OUTPUT);
    digitalWrite(relay, HIGH);   

    WiFi.mode(WIFI_STA);
    Serial.begin(115200);
    
    bool res;
    
    res = wm.autoConnect("IoTFan","password"); // password protected ap

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
        digitalWrite(relay, HIGH);   
        wm.server->send(200, "application/json","{\"status\": \"Power on relay\"}");
      });
      wm.server->on("/iot-fan/output/off", [&]() {
        digitalWrite(relay, LOW);
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
      Serial.println("on");
      digitalWrite(relay, HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(relay, LOW);
    }
  }
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
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
