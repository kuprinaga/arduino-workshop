/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <ESP8266WiFi.h>DHT22

int buttonId = 5;

const char* ssid     = "MSFTFLUX";
const char* password = "weareinflux";

const char* host = "emil.fi";
const char* id   = "TeamWolfNastya";

void setup() {
  Serial.begin(115200);
  delay(10);
  
  pinMode(LED_BUILTIN, OUTPUT); 
  
  pinMode(buttonId, INPUT_PULLUP); 

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

int value = 0;

void loop() {
  ++value;

  if(digitalRead(buttonId))
  {
    digitalWrite(LED_BUILTIN, 1);
  
    Serial.print("connecting to ");
    Serial.println(host);
  
    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 42069;
    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      return;
    }

    String json = "{\"text\":\"Hello Matti!\", \"username\":\"KaliaBot\", \"icon_emoji\":\":beer:\" }";
    
    // This will send the request to the server
    client.print(String("POST ") + "/ HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" + 
                 "Content-Type: application/data\r\n" +
                 "Cache-Control: no-cache\r\n" +
                 "Content-Length: " + json.length() + "\r\n" +
                 json + "\r\n" + 
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }
    
    // Read all the lines of the reply from server and print them to Serial
    while(client.available()){
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
    
    Serial.println();
    Serial.println("closing connection");

    delay(200);
  }
}

