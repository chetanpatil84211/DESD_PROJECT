#include <ThingSpeak.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "WiFi.h"

 
const char* ssid = "m"; //choose your wireless ssid
const char* password =  "00000000";

//---------Channel Details---------//
unsigned long counterChannelNumber = 2392330;            // Channel ID
const char * myCounterReadAPIKey = "44XE3OWD9TDMQOUR"; // Read API Key
const int FieldNumber1 = 1;  // The field you wish to read
const int FieldNumber2 = 2; 
const int FieldNumber3 = 3; // The field you wish to


WiFiClient  client;

// Define LCD address and dimensions
const int SDA_PIN = 21;
const int SCL_PIN = 22;
const int address = 0x27; // Replace with your LCD's address
const int cols = 20;
const int rows = 4;
LiquidCrystal_I2C lcd(address, cols, rows);



// Define two tasks for Blink & AnalogRead.
void Task1( void *pvParameters );
void Task2( void *pvParameters );
void Task3( void *pvParameters );

// The setup function runs once when you press reset or power on the board.
void setup() {

  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);

  Wire.begin(SDA_PIN, SCL_PIN);
   // Initialize LCD
  lcd.init();
  lcd.backlight(); // Turn on backlight (optional)

  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    lcd.setCursor(0, 0);
    lcd.print("Connecting to wiFi..");
  }
  lcd.setCursor(0, 1);
  lcd.print("WiFi connected.");

  delay(500);
  lcd.clear();
 

  // Show welcome message
  lcd.setCursor(0, 0);
  lcd.print("AirQualityMonitoring");

  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("---MQTT Subcriber---");
  delay(1000);
  lcd.setCursor(0, 2);
  lcd.print("Mic_controller ESP32");
  delay(1000);
  lcd.setCursor(0, 3);
  lcd.print("---Using FreeROTS---");
  delay(3000);

  lcd.clear();

  // Initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  // Set up two tasks to run independently.
  uint32_t delay = 1000; // Delay between changing state on LED pin
 
  xTaskCreate(
    Task1
    ,  "Task1" // A name just for humans
    ,  2048        // The stack size can be checked by calling `uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);`
    ,  (void*) &delay // Task parameter which can modify the task behavior. This must be passed as pointer to void.
    ,  2  // Priority
    ,  NULL // Task handle is not used here - simply pass NULL
    );
 

}

void loop(){
  
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void Task1(void *pvParameters){  // This is a task.
  uint32_t ndelay = *((uint32_t*)pvParameters);


  int MQ2_sensor = 300;
  int MQ7_sensor = 50;
  int MQ135_sensor = 150;

  while(1)
  {
      //------------------MQ-2---------------------------
      MQ2_sensor = ThingSpeak.readLongField(counterChannelNumber, FieldNumber1, myCounterReadAPIKey);
      int statusCode = ThingSpeak.getLastReadStatus();
      if (statusCode == 200)
      {
       // MQ2_sensor = 200;  
      }
      else
      {
        MQ2_sensor = 404;
      }
      delay(200);
      
      //------------------MQ-7---------------------------
      MQ7_sensor = ThingSpeak.readLongField(counterChannelNumber, FieldNumber1, myCounterReadAPIKey);
      statusCode = ThingSpeak.getLastReadStatus();
      if (statusCode == 200)
      {
       // MQ7_sensor = 200;  
      }
      else
      {
        MQ7_sensor = 404;
      }
      delay(200);

      //------------------MQ-135---------------------------
      MQ135_sensor = ThingSpeak.readLongField(counterChannelNumber, FieldNumber1, myCounterReadAPIKey);
      statusCode = ThingSpeak.getLastReadStatus();
      if (statusCode == 200)
      {
       // MQ135_sensor = 200;  
      }
      else
      {
        MQ135_sensor = 404;
      }
      delay(200);

      //------------------------------------------------------------------
      for (;;)
      { // A Task shall never return or exit.
      lcd.setCursor(0, 0);
      lcd.print("____________________");
      // Your code here
      lcd.setCursor(0, 1);
      lcd.print("|MQ-2 |MQ-7 |MQ-135|");

      lcd.setCursor(0, 2);
      lcd.print("|");


    //mq2
      lcd.setCursor(1, 2);
      lcd.print(MQ2_sensor);

      lcd.setCursor(6, 2);
      lcd.print("|");
      //mq7
      lcd.setCursor(7, 2);
      lcd.print(MQ7_sensor);


      lcd.setCursor(12, 2);
      lcd.print("|");
      //mq135
      lcd.setCursor(13, 2);
      lcd.print(MQ135_sensor);

      lcd.setCursor(19, 2);
      lcd.print("|");
      lcd.setCursor(0, 3);
      lcd.print("|_____|_____|______|");
      //delay(ndelay);
      }
  }  
}




