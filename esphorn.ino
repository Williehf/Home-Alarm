//Components: ESP-01,relay module, buzzer, 5v power supply

#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define RELAY         0
#define LED             13

const char* ssid = "Your WiFi name";
const char* password =  "Your WiFi Password";
const char* mqttServer = "Ip Address where the MQTT is installed Example 10.0.0.10 or 192.168.0.10";
const int mqttPort =1883; //default port for MQTT
const char* mqttUser = "Enter MQTT user name";
const char* mqttPassword = "Enter MQTT password";
WiFiClient espAlarm; // You can change espClient for any name
PubSubClient client(espAlarm);

bool Status = false;
int kUpdFreq = 1;
int kRetries = 10;

unsigned long TTasks;

void setup() {
  // put your setup code here, to run once:
  pinMode(RELAY, OUTPUT);
  pinMode(LED, OUTPUT);
  

  digitalWrite(0, 0);
  digitalWrite(LED, HIGH);
  
  //digitalWrite(SENSOR, 0);
  
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  while (!client.connected()) {
    
    if(client.connect("ESP8266Alarm", mqttUser, mqttPassword)) 
{
  
    }else{
      Serial.print("failed state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  
  client.subscribe("home/alarm/set");// here is where you later add a wildcard
}

void callback(char* topic, byte* payload, unsigned int length)
{
  
  for(int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  if ((char)payload[0] == '1'){ //In Home Assistant when turn "ON", send "1" to turn "ON" the alarm
    digitalWrite(0, 1);
    digitalWrite(LED, LOW);
    client.publish("home/alarm","on", true);
    }
    else{//In Home Assistant when turn "off", send "0" to turn "Off" the alarm
      digitalWrite(0, 0);
      digitalWrite(LED, HIGH);
      client.publish("home/alarm","off", true);
      }
  Status = true;
  }

void sendStatus()
{
  if (Status){
    if (digitalRead(LED) == LOW){
    
    }else{
     
    }
    Status = false;
  }
 }

void checkConnection(){
  if (WiFi.status() != WL_CONNECTED){
    if (client.connected()){
      //Serial.println("connected in check");
      }
    else{
      reconnect();
      }
  }
    else{
      ESP.restart();
      }
   }

void reconnect() {
  // Loop until reconnected
  while (!client.connected()) {
    if (client.connect("ESP8266Alarm", mqttUser, mqttPassword)){
       client.subscribe("home/alarm/set");
    } else {
      Serial.print(client.state());
      delay(5000);
    }
  }
}

void timedTasks() {
  if ((millis() > TTasks + (kUpdFreq * 60000)) || (millis() < TTasks)) {
    TTasks = millis();
    checkConnection();
  }
}

void loop() {
  
  // put your main code here, to run repeatedly:
 if (!client.connected()) {
    reconnect();
  } 
  if(!client.loop())
  client.connect("ESP8266Alarm", mqttUser, mqttPassword);
  delay(200);
  client.connect("ESP8266Client");
  delay(500);
  timedTasks();
  sendStatus(); 
}
