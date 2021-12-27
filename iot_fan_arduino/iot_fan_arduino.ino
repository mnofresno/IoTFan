#include <WiFiManager.h>

const int relay = 26;

WiFiManager wm;

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

  
      wm.server->on("/poweron", [&]() {
        digitalWrite(relay, HIGH);   
        wm.server->send(200, "application/json","{\"status\": \"Power on relay\"}");
      });
      wm.server->on("/poweroff", [&]() {
        digitalWrite(relay, LOW);
        wm.server->send(200, "application/json", "{\"status\": \"Power off relay\"}");
      });
    }

}

void loop() {
    wm.process();


    // put your main code here, to run repeatedly:   
}
