#include <ESP8266WiFi.h> 
#include <WiFiClient.h> 

const char* ssid = "Iiraa"; 
const char* password = "Isha2004"; 
const char* host = "prayalabs.com"; 
const int httpPort = 80; 

const int TL1_GREEN_PIN = D1; 
const int TL1_YELLOW_PIN = D2; 
const int TL1_RED_PIN = D3; 
const int TL2_GREEN_PIN = D4; 
const int TL2_YELLOW_PIN = D5; 
const int TL2_RED_PIN = D6; 

const int GREEN_DURATION = 10000; // 10 seconds 
const int YELLOW_DURATION = 5000; // 5 seconds 

void setup() { 
  pinMode(TL1_GREEN_PIN, OUTPUT); 
  pinMode(TL1_YELLOW_PIN, OUTPUT); 
  pinMode(TL1_RED_PIN, OUTPUT); 
  pinMode(TL2_GREEN_PIN, OUTPUT); 
  pinMode(TL2_YELLOW_PIN, OUTPUT); 
  pinMode(TL2_RED_PIN, OUTPUT); 
  pinMode(LED_BUILTIN, OUTPUT); 
  
  Serial.begin(115200); 
  
  Serial.println(); 
  Serial.print("Connecting to "); 
  Serial.println(ssid); 
  
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500); 
    Serial.print("."); 
  } 
  
  Serial.println(); 
  Serial.println("WiFi connected"); 
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP()); 
} 

void loop() { 
  while (true) { 
    // TL1 and TL2 green, others red
    digitalWrite(TL1_GREEN_PIN, HIGH); 
    digitalWrite(TL1_YELLOW_PIN, LOW); 
    digitalWrite(TL1_RED_PIN, LOW); 
    digitalWrite(TL2_GREEN_PIN, HIGH); 
    digitalWrite(TL2_YELLOW_PIN, LOW); 
    digitalWrite(TL2_RED_PIN, LOW); 
    delay(GREEN_DURATION); 
    
    // TL1 and TL2 yellow, others red
    digitalWrite(TL1_GREEN_PIN, LOW); 
    digitalWrite(TL1_YELLOW_PIN, HIGH); 
    digitalWrite(TL1_RED_PIN, LOW); 
    digitalWrite(TL2_GREEN_PIN, LOW); 
    digitalWrite(TL2_YELLOW_PIN, HIGH); 
    digitalWrite(TL2_RED_PIN, LOW); 
    delay(YELLOW_DURATION); 
    
    // TL1 and TL2 red, others green
    digitalWrite(TL1_GREEN_PIN, LOW); 
    digitalWrite(TL1_YELLOW_PIN, LOW); 
    digitalWrite(TL1_RED_PIN, HIGH); 
    digitalWrite(TL2_GREEN_PIN, LOW); 
    digitalWrite(TL2_YELLOW_PIN, LOW); 
    digitalWrite(TL2_RED_PIN, HIGH); 
    delay(GREEN_DURATION); 
    
    // TL1 and TL2 red, others yellow
    digitalWrite(TL1_GREEN_PIN, LOW); 
    digitalWrite(TL1_YELLOW_PIN, LOW); 
    digitalWrite(TL1_RED_PIN, HIGH); 
    digitalWrite(TL2_GREEN_PIN, LOW); 
    digitalWrite(TL2_YELLOW_PIN, HIGH); 
    digitalWrite(TL2_RED_PIN, LOW); 
    delay(YELLOW_DURATION); 
    
    // Read LED states
    int TL1Green = digitalRead(TL1_GREEN_PIN); 
    int TL1Yellow = digitalRead(TL1_YELLOW_PIN); 
    int TL1Red = digitalRead(TL1_RED_PIN); 
    int TL2Green = digitalRead(TL2_GREEN_PIN); 
    int TL2Yellow = digitalRead(TL2_YELLOW_PIN); 
    int TL2Red = digitalRead(TL2_RED_PIN); 
    
    if (WiFi.status() == WL_CONNECTED) { 
      WiFiClient client; 
      Serial.print("Connecting to "); 
      Serial.println(host); 
      
      if (!client.connect(host, httpPort)) { 
        Serial.println("Connection failed"); 
        return; 
      } 
      
      String queryString = "/rest/api/iot?"; 
      queryString += "east[signal]=red&"; 
      queryString += "east[duration]=5&"; 
      queryString += "east[status]=" + String(TL1Red ? "on" : "off") + "&"; 
      queryString += "west[signal]=red&"; 
      queryString += "west[duration]=5&"; 
      queryString += "west[status]=" + String(TL2Red ? "on" : "off") + "&"; 
      queryString += "north[signal]=green&"; 
      queryString += "north[duration]=5&"; 
      queryString += "north[status]=" + String(TL2Green ? "on" : "off") + "&"; 
      queryString += "south[signal]=green&"; 
      queryString += "south[duration]=5&"; 
      queryString += "south[status]=" + String(TL1Green ? "on" : "off") + "&"; 
      queryString += "buildinglights=on"; 
      
      client.print(String("GET ") + queryString + " HTTP/1.1\r\n" + 
                   "Host: " + host + "\r\n" + 
                   "Connection: close\r\n\r\n"); 
      
      Serial.println("Request sent"); 
      
      while (client.connected()) { 
        String line = client.readStringUntil('\r'); 
        Serial.println(line); 
      } 
      
      Serial.println("Response received"); 
      client.stop(); 
    } 
    
    delay(1000); 
  } 
}
