#include <WiFiManager.h>
//#include ".credentials.h"
#include <PubSubClient.h>
#include <WiFi.h>
#include <esp_task_wdt.h>

#define WDT_TIMEOUT 3

const char* mqtt_server = "192.168.1.1";
const char* topic_to_subscribe = "iot-fan/output";
const int relay = 26;

WiFiClient espClient;
WiFiManager wm;
PubSubClient client(espClient);

int i = 0;
int last = millis();

void setup() {
    esp_task_wdt_init(WDT_TIMEOUT, true); //enable panic so ESP32 restarts
    esp_task_wdt_add(NULL); //add current thread to WDT watch

    pinMode(relay, OUTPUT);
    setPowerOn();  

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
    reset_wdt();
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
  Serial.println("on - NC relay is OFF");
  digitalWrite(relay, LOW);
}

void setPowerOff() {
  Serial.println("off - NC relay is ON");
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

void reset_wdt() {
  // resetting WDT every 2s, 5 times only
  if (millis() - last >= 2000 && i < 5) {
      Serial.println("Resetting WDT...");
      esp_task_wdt_reset();
      last = millis();
      i++;
      if (i == 5) {
        Serial.println("Stopping WDT reset. CPU should reboot in 3s");
      }
  }
}
