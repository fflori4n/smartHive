#include <SoftwareSerial.h> /// probably will be removed
#include "RS485Com.h"

#define RO_PIN 10
#define DI_PIN 11
#define DE_PIN 12

SoftwareSerial Serial485 (RO_PIN, DI_PIN); // RX, TX
RS485Com serial;

int dht0Temp = -999;  /// int representation of float value, truncated to 1 decimal, and multiplied by 10, would be -99.9
int dht0Humi = -999;
int dht1Temp = -999;
int dht1Humi = -999;
int dht2Temp = -999;
int dht2Humi = -999;



void setup() {
  Serial.begin(9600); 
}

void loop() {
  // put your main code here, to run repeatedly:
  if(serial.checkInbuf() == 0){
    serial.parseBuff();
  }
  delay(100);
  Serial.println("Main loop");

}
