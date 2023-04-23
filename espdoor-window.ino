//Components: ESP-01, 3.3v regulator, reed switch, magnet, 3.3 battery

#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Your WiFi name";
const char* password =  "Your WiFi Password";
const char* mqttServer = "Ip Address where the MQTT is installed Example 10.0.0.10 or 192.168.0.10";
const int mqttPort =1883; //default port for MQTT
const char* mqttUser = "Enter MQTT user name";
const char* mqttPassword = "Enter MQTT password";
WiFiClient espClient;// You can change espClient for any name
PubSubClient client(espClient);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
}
  client.setServer(mqttServer, mqttPort);
  while (!client.connected()) {
    if(client.connect("ESP8266Client", mqttUser, mqttPassword )) 
{
  Serial.println("connected");
  client.publish("home/sensor/door","open"); // Report to Home Assistant that the door is opened
  client.subscribe("home/sensor/door/set");// here is where you later add a wildcard
  delay(1000);
  client.publish("home/sensor/door","close");//wait 1 second and the refresh the icon to "Close" in Home Assistant for another event
  delay(1000);
  
    }else{
      Serial.print("failed state ");
      Serial.print(client.state());
      }
  }
  ESP.deepSleep(0, WAKE_RF_DEFAULT);
  espClient.stop();}

void loop() {
  // put your main code here, to run repeatedly:
  // In this case for window and door once the circuit is actived then put to deepsleep the ESP-01
 ESP.deepSleep(0, WAKE_RF_DEFAULT);}
