#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

// Replace with your network credentials
const char* ssid = "Galexy";
const char* password = "123456789";

// Replace with your ThingSpeak channel details
const char* server = "api.thingspeak.com";
unsigned long channelID = 2392330;
String writeAPIKey = "F85MCW7PUA49OZUU";

#define rxPin D6
#define txPin D5
SoftwareSerial mySerial(rxPin, txPin); 

String str;
int i;

WiFiClient client;

void setup() {
  Serial.begin(115200);
  mySerial.begin(115200); 
  delay(10);

  // Connect to WiFi
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
}

void loop() {
  // Get sensor data (replace with your actual sensor readings)
  //float sensor1Data = 25.5;
  //float sensor2Data = 71.0;
  //float sensor3Data = 71.0;

  // Prepare the data string to send
  //String data = String("field1=") + String(sensor1Data) + "&field2=" + String(sensor2Data)+ "&field3=" + String(sensor3Data);
  //i=0;
  memset(&str,0,sizeof(str));
  while(1){
  if (mySerial.available()){ 
    char incomingByte =mySerial.read();
        if (incomingByte!='\n')
            str =str+incomingByte ;
        else
           break;
  }
  }
  Serial.println(str);
  // Send the data to ThingSpeak
  if (client.connect(server, 80)) {
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + writeAPIKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(str.length());
    client.print("\n\n");
    client.print(str);
    Serial.println("Data sent to ThingSpeak");
  }

  client.stop();

 
  delay(1000); // Delay between updates (20 seconds)
}
