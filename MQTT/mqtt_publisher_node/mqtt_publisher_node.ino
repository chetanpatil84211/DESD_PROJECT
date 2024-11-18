#include <ESP8266WiFi.h>  
#include <PubSubClient.h>
#include <WiFiClient.h>

char ssid[] = "MOSFET";            
char pass[] = "dont_ask_me";
const char* mqtt_cliend_ID= "LCYnED0JIwMMPAUMMBIFOCg";
const char* mqtt_username = "LCYnED0JIwMMPAUMMBIFOCg";
const char* mqtt_password = "8YZx9/JMRjG4pKTelYkZ0PoH";
const char* mqtt_server = "mqtt3.thingspeak.com";
const char* publishTopic ="channels/1925243/publish"; 
const char* subscribeTopicFor_Command_1="channels/1925243/subscribe/fields/+";

WiFiClient espClient;
PubSubClient client(espClient); 

void setup() {
  pinMode(2,OUTPUT);
  pinMode(16,OUTPUT);
  Serial.begin(115200);
  while (!Serial) delay(1);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();
  char buffer[50]="";
  if (Serial.available()) {
    Serial.readBytesUntil('\n', buffer, 50);
    Serial.println(buffer);
    publishMessage(publishTopic,buffer,true);
  }
 }

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(mqtt_cliend_ID, mqtt_username, mqtt_password)) {  
      Serial.println("connected");
      digitalWrite(16,HIGH);
      client.subscribe(subscribeTopicFor_Command_1); 
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5  seconds"); 
      delay(5000);
    }
  }
}
void callback(char* topic, byte* payload, unsigned int length) {
  String incommingMessage = "";
  for (int i = 0; i < length; i++) 
  incommingMessage+=(char)payload[i];
  Serial.println("Message arrived ["+String(topic)+"]"+incommingMessage);
}

void publishMessage(const char* topic, String payload , boolean retained){
  if (client.publish(topic, payload.c_str()))
      Serial.println("Message publised ["+String(topic)+"]: "+payload);
}

void setup_wifi() {
  Serial.print("\n Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  digitalWrite(2,HIGH);
  Serial.println("\nWiFi connected\nIP address: ");
  Serial.println(WiFi.localIP());
}
