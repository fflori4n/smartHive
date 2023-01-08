#define _TEMP_STRLEN 100

#include "RS485Com.h"
#include "BasicHiveSensor.h"
#include "sim7000.h"
#include "sensHubInfo.h"
#include "ReadVoltage.h"
#include "Mqtt.h"



char mqttPayloadBuffer[500] = "{";
char mqttPayloadBuff[500] = "";
char tempStrBuffer[_TEMP_STRLEN];

SIM7000 gsmModem = SIM7000();
RS485Com com485 = RS485Com();

BasicHiveSensor sensorA((char)65,"BHS_A", com485);
BasicHiveSensor sensorB((char)66,"BHS_B", com485);


BasicHiveSensor* bHSensList[2] = {&sensorA, &sensorB};




void setup()  
{
  //simModem.begin(15200, SERIAL_8N1, 16, 17); 
  Serial.begin(115200);
  Serial.println("hello serial");
  gsmModem.init();

  /// pin for reading voltages
  pinMode(VPPIN, INPUT);
  analogSetWidth(11);
  analogReadResolution(11);
  /// \pin for reading voltages
  
  //Serial.println("GSM COM DUMP:");
 // sendHubStatusMqtt(gsmModem);
} 
void loop()  
{ 
 // readVoltages();
  //delay(1000);
  //delay(10000);
  //Serial.println("loop");

  //Serial.println("GSM COM DUMP:");
  //sendHubStatusMqtt(gsmModem);
  //delay(600000);
  //com485.sendDataPullMsg('A', 'B');
  
  
/*  com485.pollSensor((char)65, (char)70);
  delay(1500);
  Serial.println(com485.checkInbuf());
  delay(10000);*/

  /*com485.pollSensor((char)65,(char)70);
  delay(2000);
  com485.checkInbuf();
  delay(5000);
  com485.pollSensor((char)66,(char)70);
  delay(2000);
  com485.checkInbuf();
  delay(5000);

  delay(1000);*/
  sensorA.update();
  delay(2000);
  sensorB.update();
  delay(2000);
  
  sendMqttStatusMsg(gsmModem,mqttPayloadBuff);
  sendMqttBHSensorMsg(gsmModem,mqttPayloadBuff, bHSensList, 2);
  
  //sensorA.addMqttTags(mqttPayloadBuffer, tempStrBuffer);
  //sensorB.addMqttTags(mqttPayloadBuffer, tempStrBuffer);
  //Serial.println(mqttPayloadBuffer);
 // mqttPayloadBuffer = "";
  //sendHubStatusMqtt(gsmModem);
  delay(20000);
}

int sendATCommand(const char* command, const char* response ){
  

}
