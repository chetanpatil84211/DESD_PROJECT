//=====================================================================
///////////////////////////////  Publisher  ///////////////////////////
//=====================================================================
#include <ESP8266WiFi.h>  
#include <PubSubClient.h>
#include <WiFiClient.h>

char ssid[] = "MOSFET";
char pass[] = "dont_ask_me";
const char* channel_ID = "1925243";  
const char* mqtt_cliend_ID = "LCYnED0JIwMMPAUMMBIFOCg";
const char* mqtt_username = "LCYnED0JIwMMPAUMMBIFOCg";
const char* mqtt_password = "XIOR2KULR82xhk+9Q0tVTeo8";
const char* mqtt_server = "mqtt3.thingspeak.com";
const char* publishTopic = "channels/channel_ID/publish"; 
const char* subscribeTopicFor_Command_1="channels/channel_ID/subscribe/fields/field1";   
const char* subscribeTopicFor_Command_2="channels/channel_ID/subscribe/fields/field2";
const char* subscribeTopicFor_Command_3="channels/channel_ID/subscribe/fields/field3";
char buffer[40] = "";

WiFiClient espClient;
PubSubClient client(espClient); 

void setup_wifi() {
  delay(10);
  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    digitalWrite(LED_BUILTIN,LOW);
  }
  randomSeed(micros());
  Serial.println("\nWiFi connected\nIP address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(LED_BUILTIN,HIGH);
}

void reconnect() {

  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(mqtt_cliend_ID, mqtt_username,mqtt_password)) {  
      Serial.println("connected");
      client.subscribe(subscribeTopicFor_Command_1);
      client.subscribe(subscribeTopicFor_Command_2);
      client.subscribe(subscribeTopicFor_Command_3);  
      } 
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void messageReceived(String &topic, String &payload) {
 Serial.println("incoming: " + topic + " - " + payload);
  if (topic==subscribeTopicFor_Command_1){
        Serial.println("Command 1 = "+payload);
        int v = payload.toInt();
        if (v>0) digitalWrite(16,HIGH);
        else digitalWrite(16,LOW);
   }
}

void setup() {
  pinMode(LED_BUILTIN,OUTPUT);
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
      if (Serial.available())
    {
      Serial.readBytesUntil('\n', buffer, 40);
      Serial.println(buffer);
      publishMessage(publishTopic,buffer,true);
    }
    char buffer[40] = "";

 }

void callback(char* topic, byte* payload, unsigned int length) {
  String incommingMessage = "";
  for (int i = 0; i < length; i++) incommingMessage+=(char)payload[i];
  
  Serial.println("Message arrived ["+String(topic)+"]"+incommingMessage);

    if( strcmp(topic,subscribeTopicFor_Command_1) == 0){
     if (incommingMessage.equals("1")) digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on 
     else digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off 
  }
}

void publishMessage(const char* topic, String payload , boolean retained){
  if (client.publish(topic, payload.c_str()))
      Serial.println("Message publised ["+String(topic)+"]: "+payload);
}
