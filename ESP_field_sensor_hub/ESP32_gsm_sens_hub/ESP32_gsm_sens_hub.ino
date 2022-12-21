#define _TEMP_STRLEN 100

#include "sim7000.h"
#include "sensHubInfo.h"
#include "Mqtt.h"


SIM7000 gsmModem = SIM7000();


void setup()  
{
  //simModem.begin(15200, SERIAL_8N1, 16, 17); 
  Serial.begin(115200);
  Serial.println("hello serial");
  gsmModem.init();
  Serial.println("GSM COM DUMP:");
  sendHubStatusMqtt(gsmModem);
} 
void loop()  
{ 
  
  delay(10000);
  Serial.println("loop");
}

int sendATCommand(const char* command, const char* response ){
  

}
