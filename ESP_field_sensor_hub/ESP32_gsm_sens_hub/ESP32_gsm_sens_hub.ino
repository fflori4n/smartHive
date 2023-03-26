#define _TEMP_STRLEN 100

#include "RS485Com.h"
#include "BasicHiveSensor.h"
#include "sim7000.h"
#include "sensHubInfo.h"
#include "ReadVoltage.h"
#include "Mqtt.h"
#include "ESPtime.h"

#define SENS_PWR_ON 23
#define SENS_PWR_OFF 22


char mqttPayloadBuffer[500] = "{";
char mqttPayloadBuff[500] = "";
char tempStrBuffer[_TEMP_STRLEN];

SIM7000 gsmModem = SIM7000();
RS485Com com485 = RS485Com();

BasicHiveSensor sensorA((char)65,"BHS_A", com485);
BasicHiveSensor sensorB((char)66,"BHS_B", com485);


BasicHiveSensor* bHSensList[2] = {&sensorA, &sensorB};
ESPtime hubTime;

void setup()  
{
//  simModem.begin(15200, SERIAL_8N1, 16, 17); 
  Serial.begin(115200);
  Serial.println("hello serial");
  gsmModem.init();
  hubTime.useModem(gsmModem);
  delay(2000);
  hubTime.setUnixtime(1679809221);

  /// pin for reading voltages
  pinMode(VPPIN, INPUT);
  analogSetWidth(11);
  analogReadResolution(11);
  /// \pin for reading voltages

  pinMode(SENS_PWR_ON, OUTPUT);
  pinMode(SENS_PWR_OFF, OUTPUT);
  digitalWrite(SENS_PWR_ON, LOW);
  digitalWrite(SENS_PWR_OFF, LOW);
  
  //Serial.println("GSM COM DUMP:");
 // sendHubStatusMqtt(gsmModem);
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
  //readVoltages();
  //delay(1000);
  //delay(10000);
  //Serial.println("loop");

  //Serial.println("GSM COM DUMP:");
  //sendHubStatusMqtt(gsmModem);
  //delay(600000);
  //com485.sendDataPullMsg('A', 'B');
  hubTime.printLocalTime();
  hubTime.updateTimeIfNeeded();
  setSensorPwr(true);
  delay(5000);
  sensorA.update();
  delay(2000);
  sensorB.update();
  delay(2000);
  setSensorPwr(false);
  
  //delay(20000);

#ifdef _SEND_MQTT
  sendMqttStatusMsg(gsmModem,mqttPayloadBuff, "RTU0/RTU_INFO");
  delay(20000);
#endif

  /*for(int i=0; i<10; i++){                                  /// has to be sent a few times to set com speed
    if(gsmModem.atPrint("AT\r","OK",500) == 0){
      break;
    }
  }
    gsmModem.atPrint("AT&F\r","OK");
    delay(30000);
    for(int i=0; i<10; i++){                                  /// has to be sent a few times to set com speed
    if(gsmModem.atPrint("AT\r","OK",500) == 0){
      break;
    }
  }*/
   /* gsmModem.atPrint("AT+CPIN?\r","OK");
    gsmModem.atPrint("AT+CMEE=1\r","OK", 10000);
    gsmModem.atPrint("AT+CNMP=13\r","OK");
    gsmModem.atPrint("AT+CFUN=1\r","OK");
    gsmModem.atPrint("AT+CPSI?\r","OK");
    gsmModem.atPrint("AT+CGNSPWR=0\r","OK");
    
    gsmModem.atPrint("AT+COPS?\r","OK", 10000);
    gsmModem.atPrint("AT+CREG?\r","+CREG: 0,1", 30000);
    gsmModem.atPrint("AT+CSQ\r","OK");
    gsmModem.atPrint("AT+CMEE=2\r","OK");
    gsmModem.atPrint("ATX 0\r","OK");
    //gsmModem.atPrint("ATD *100#;\r","OK");
   // gsmModem.atPrint("ATZ\r","OK");
    
    gsmModem.atPrint("AT+GMR\r","OK");*/
   // gsmModem.atPrint("ATD0640504964\r","OK");
   // gsmModem.atPrint("ATD0640504964;\r","OK");
  //sendMqttBHSensorMsg(gsmModem,mqttPayloadBuff,"RTU0/BHSENS", bHSensList, 2);
  delay(10000);
  
  //sensorA.addMqttTags(mqttPayloadBuffer, tempStrBuffer);
  //sensorB.addMqttTags(mqttPayloadBuffer, tempStrBuffer);
  //Serial.println(mqttPayloadBuffer);
 // mqttPayloadBuffer = "";
  //sendHubStatusMqtt(gsmModem);
  //delay(20000);
}
