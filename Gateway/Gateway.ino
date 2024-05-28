#include <Console.h>
#include <Process.h>
#include <SPI.h>
#include <RH_RF95.h>

RH_RF95 rf95;

char DeviceID[25] = "GATEX001";

char receivedData[50];
int threshold = 10;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!rf95.init()) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  
  Serial.println("Ready to Receive!");
  rf95.setFrequency(865);
  rf95.setSpreadingFactor(12);
  rf95.setSignalBandwidth(125E3);
  rf95.setCodingRate4(8);
  rf95.setTxPower(20, false);
}

void loop() {
  if (rf95.available()) {
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    if (rf95.recv(buf, &len)) {
      buf[len] = '\0';
      strcpy(receivedData, (char*)buf);

      Serial.print("Received data: ");
      Serial.println(receivedData);

      int val_x, val_y, val_z;
      sscanf(receivedData, "%d,%d,%d", &val_x, &val_y, &val_z);

      if (val_x > threshold || val_y > threshold || val_z > threshold) {
        Serial.println("Fault detected! Threshold exceeded.");
      }
    }
  }
}
