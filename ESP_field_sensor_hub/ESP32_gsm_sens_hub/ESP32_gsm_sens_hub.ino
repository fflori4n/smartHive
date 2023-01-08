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
//  simModem.begin(15200, SERIAL_8N1, 16, 17); 
  Serial.begin(115200);
  Serial.println("hello serial");
  gsmModem.init();
  delay(2000);

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
  
  sensorA.update();
  delay(2000);
  sensorB.update();
  delay(2000);
  
  sendMqttStatusMsg(gsmModem,mqttPayloadBuff, "RTU0/RTU_INFO");
  sendMqttBHSensorMsg(gsmModem,mqttPayloadBuff,"RTU0/BHSENS", bHSensList, 2);
  delay(60000 * 5);
  
  //sensorA.addMqttTags(mqttPayloadBuffer, tempStrBuffer);
  //sensorB.addMqttTags(mqttPayloadBuffer, tempStrBuffer);
  //Serial.println(mqttPayloadBuffer);
 // mqttPayloadBuffer = "";
  //sendHubStatusMqtt(gsmModem);
  //delay(20000);
}
