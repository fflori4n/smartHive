#define _TEMP_STRLEN 100
#define MQTT_PAYLOAD_BUFF_LEN 800

#include <WiFi.h>
#include <PubSubClient.h>
#include "RS485Com.h"
#include "BasicHiveSensor.h"
#include "sim7000.h"
#include "sensHubInfo.h"
#include "ReadVoltage.h"
#include "SolarLogger.h"
#include "wifiFunctions.h"
#include "Mqtt.h"
#include "ESPtime.h"

#define SENS_PWR_ON 23
#define SENS_PWR_OFF 22

char mqttPayloadBuffer[MQTT_PAYLOAD_BUFF_LEN] = "{";
char mqttPayloadBuff[MQTT_PAYLOAD_BUFF_LEN] = "";
char tempStrBuffer[_TEMP_STRLEN];

bool sensorsUpToDate = false;
bool initialOnlineReport = false;

SIM7000 gsmModem = SIM7000();
RS485Com com485 = RS485Com();

BasicHiveSensor sensorA((char)65,"BHS_A", com485);
BasicHiveSensor sensorB((char)66,"BHS_B", com485);


BasicHiveSensor* bHSensList[2] = {&sensorA, &sensorB};
ESPtime hubTime;

bool lowBattery = false;

void setup()  
{
//  simModem.begin(15200, SERIAL_8N1, 16, 17); 
  Serial.begin(115200);
  Serial.println("hello serial");
  gsmModem.init();
  hubTime.useModem(gsmModem);
  solarLogSerial.begin(4800);
  delay(8000);

  /// pin for reading voltages
  pinMode(VNPIN, INPUT);
  analogSetWidth(11);
  analogReadResolution(11);

  pinMode(SENS_PWR_ON, OUTPUT);
  pinMode(SENS_PWR_OFF, OUTPUT);
  digitalWrite(SENS_PWR_ON, LOW);
  digitalWrite(SENS_PWR_OFF, LOW);
  
  setSensorPwr(true);
  hubTime.updateTimeIfNeeded(true);
} 

void setSensorPwr(bool isON){
  if(isON){
    digitalWrite(SENS_PWR_ON, HIGH);
    delayMicroseconds(100);
    digitalWrite(SENS_PWR_ON, LOW);
  }
  else{
    digitalWrite(SENS_PWR_OFF, HIGH);
    delay(10);
    digitalWrite(SENS_PWR_OFF, LOW);
  }
}

void loop()  
{ 
  readVoltages();

  bool powerSavingEnabled = isPowerSavingEnabled();
  //Serial.println();
  if(!sensorsUpToDate && hubTime.isTimeForSensorUpdate()){
    if(!powerSavingEnabled){
      setSensorPwr(true);
      delay(20000);
      sensorA.update();
      sensorB.update();
      setSensorPwr(false);
    }
    sensorsUpToDate = true;
  }
  else if(hubTime.isTimeForSend() || !initialOnlineReport){
    Serial.println("MQTT SEND:");
    sendMqttStatusMsg(gsmModem,mqttPayloadBuff, "RTU0/RTU_INFO");
    delay(1000);
    if(!powerSavingEnabled){
      sendMqttBHSensorMsg(gsmModem,mqttPayloadBuff,"RTU0/BHSENS", bHSensList, 2);
    }
    sensorsUpToDate = false;
    initialOnlineReport = true;
  }
  //hubTime.printLocalTime();
  hubTime.updateTimeIfNeeded();
  delay(5000);
}
