#include "ThingSpeak.h" 
#include <ESP8266WiFi.h> 

const char* ssid     = "WIFI-NAME"; 
const char* password = "WIFI-PASSWORD"; 
unsigned long channel = 2048279; 

unsigned int led1 = 1; 
unsigned int led2 = 2; 
unsigned int led3 = 3; 

int ENA = D7; 
int IN1 = D1; 
int IN2 = D2; 

WiFiClient client; 

void setup() { 
  Serial.begin(115200); 
  delay(100); 
  
  pinMode(D3, OUTPUT); 
  pinMode(ENA, OUTPUT); 
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT);  
  
  digitalWrite(IN1, LOW); 
  digitalWrite(IN2, LOW); 
  digitalWrite(D3, 0); 
  
  Serial.println(); 
  Serial.println(); 
  Serial.print("Connecting to "); 
  Serial.println(ssid); 
  
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) { 
    delay(100); 
    Serial.print("."); 
  } 
  
  Serial.println(""); 
  Serial.println("WiFi connected");   
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP()); 
  Serial.print("Netmask: "); 
  Serial.println(WiFi.subnetMask()); 
  Serial.print("Gateway: "); 
  Serial.println(WiFi.gatewayIP()); 
  
  ThingSpeak.begin(client); 
} 

void loop() { 
  int led_1 = ThingSpeak.readIntField(channel, led1); 
  int led_3 = ThingSpeak.readIntField(channel, led3); 
  
  if(led_1 == 1){ 
    analogWrite(ENA, 255); 
    digitalWrite(IN1, HIGH); 
    digitalWrite(IN2, LOW);    
    Serial.println("D1 is On..!"); 
  } 
  else if(led_1 == 0){ 
    analogWrite(ENA, 1); 
    digitalWrite(IN1, LOW); 
    digitalWrite(IN2, LOW); 
    Serial.println("D1 is Off..!"); 
  } 
  
  if(led_3 == 1){ 
    digitalWrite(D3, 1); 
    Serial.println("D3 is On..!"); 
  } 
  else if(led_3 == 0){ 
    digitalWrite(D3, 0); 
    Serial.println("D3 is Off..!"); 
  } 
  
  Serial.println(led_1); 
  Serial.println(led_3); 
  
  delay(10000); 
}
