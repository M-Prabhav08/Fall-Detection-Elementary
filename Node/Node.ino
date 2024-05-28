#include <Console.h>
#include <Process.h>
#include <SPI.h>
#include <RH_RF95.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();

RH_RF95 rf95;

char DeviceID[25] = "node1";
char data[50]; 

int x = A0;
int y = A1;
int z = A3;

void setup()
{
  Serial.begin(9600);
  while (!Serial);

   if(!accel.begin())

   {

      Serial.println("No valid sensor found");

      while(1);

   }

  if (!rf95.init())
  {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  Serial.println("Ready to send");
  rf95.setFrequency(865);
  rf95.setSpreadingFactor(12);
  rf95.setSignalBandwidth(125E3);
  rf95.setCodingRate4(8);
  rf95.setTxPower(20, false);
}

void loop()
{
  sensors_event_t event;
  accel.getEvent(&event);

  int val_x = event.acceleration.x;
  float val_y = event.acceleration.y;
  float val_z = event.acceleration.z;

  Serial.print("Raw values: ");
  Serial.print(val_x);
  Serial.print(", ");
  Serial.print(val_y);
  Serial.print(", ");
  Serial.println(val_z);

  
  sprintf(data, "%d,%d,%d", val_x, static_cast<int>(val_y), static_cast<int>(val_z));
  
  Serial.print("Data is: ");
  Serial.println(data);

  Serial.println("Starting to send!");
  rf95.send((uint8_t*)data, strlen(data));
  rf95.waitPacketSent();
  Serial.println("Data sent!");

  delay(1000); 
}
