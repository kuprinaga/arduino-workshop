/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include "DHT.h"
#include <ESP8266WiFi.h>

#define DHTPIN 5     // what digital pin we're connected to

#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

const char* ssid     = "MSFTFLUX";
const char* password = "weareinflux";

const char* host = "temperatureviz.azurewebsites.net";
const char* id   = "TeamWolfNastya";

void setup() {
  Serial.begin(115200);
  delay(10);
  
  pinMode(LED_BUILTIN, OUTPUT); 
  
  dht.begin();

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

  delay(5000);
}

int value = 0;

void loop() {
  ++value;

  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C ");
  
  // We now create a URI for the request
  String url = "/data/";
  url += "?id=";
  url += id;
  url += "&value=";
  url += t;
  url += "&label=";
  url += 1;
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
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

  digitalWrite(LED_BUILTIN, HIGH);

  delay(5000);

  digitalWrite(LED_BUILTIN, LOW);
  
  delay(5000);
}

