#include <ThingSpeak.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2); // SCL - D1  SDA - D2

const char ssid[] = "MOSFET";
const char pass[] = "dont_ask_me";        
unsigned long counterChannelNumber = 1925243;
const char * myCounterReadAPIKey = "QCWMWMYQBQ3QLRXZ";
const int FieldNumber1 = 1;
const int FieldNumber2 = 2;
const int FieldNumber3 = 3;
int MQ2_sensor   = 0;
int MQ7_sensor   = 0;
int MQ135_sensor = 0;

WiFiClient  client;

void setup(){
  Serial.begin(115200);
  lcd.init();   
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("MQTT Subscriber");
  delay(2000);
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Air Quality");
  lcd.setCursor(0,1);
  lcd.print("MonitoringSystem");
  delay(2000);
  
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);

    //----------------- Network -----------------//
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Connecting to ");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("WiFi Connecting ");
    lcd.setCursor(5,1);
    lcd.print("......");
    Serial.print(ssid);
    Serial.println(" ....");
    while (WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid, pass);
      delay(5000);
    }
  Serial.println("Connected to Wi-Fi Succesfully.");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("WiFi Connected !");
  lcd.setCursor(3,1);
  lcd.print("To ");
  lcd.setCursor(6,1);
  lcd.print(ssid);
  delay(2000);
  }
}

void loop(){
  MQ2_sensor = ThingSpeak.readLongField(counterChannelNumber, FieldNumber1, myCounterReadAPIKey);
  int statusCode = ThingSpeak.getLastReadStatus();
    if (statusCode == 200){
      Serial.print("MQ-2");
      Serial.print(MQ2_sensor);
      Serial.print(",");
    }
    else{
      MQ2_sensor = 0;
      Serial.println("Unable to read channel / No internet connection");
    }
  MQ7_sensor = ThingSpeak.readLongField(counterChannelNumber, FieldNumber2, myCounterReadAPIKey);
  statusCode = ThingSpeak.getLastReadStatus();
    if (statusCode == 200){
      Serial.print("MQ-7");
      Serial.print(MQ7_sensor);
      Serial.print(",");
    }
    else{
      MQ7_sensor = 0;
      Serial.println("Unable to read channel / No internet connection");
    }
  MQ135_sensor = ThingSpeak.readLongField(counterChannelNumber, FieldNumber3, myCounterReadAPIKey);
  statusCode = ThingSpeak.getLastReadStatus();
    if (statusCode == 200){
      Serial.print("MQ-135");
      Serial.println(MQ135_sensor);
    }
    else{
      MQ135_sensor = 0;
      Serial.println("Unable to read channel / No internet connection");
    }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("MQ-2 MQ-7 MQ-135");
  lcd.setCursor(0,1);
  lcd.print(MQ2_sensor);
  lcd.setCursor(5,1);
  lcd.print(MQ7_sensor);
  lcd.setCursor(11,1);
  lcd.print(MQ135_sensor);
}
