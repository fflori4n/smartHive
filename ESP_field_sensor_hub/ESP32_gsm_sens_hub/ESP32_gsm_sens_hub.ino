#include "sim7000.h"
#include "sensHubInfo.h"
//#include <HardwareSerial.h>


SIM7000 gsmModem = SIM7000();



void getSubstring(char* destination, char* source, uint8_t startIndex, uint8_t endIndex){
  int j = 0;
  for(int i=startIndex; i< endIndex; i++){
    destination[j++] = source[i];
  }
}
void getCSQ(){
  
}
String getGNSSaGSMinfo(){
  char latitudeStr[11] = "";
  char longitudeStr[12] = "";
  char gnssHeightStr[9] = "";
  char gnssSateliteNumStr[5] = "";
  char gnssCN0Str[5] = "";
  char rssiStr[3] = "";
  uint8_t commaCount = 0;
  uint8_t prevCommaIndex = 0;

  for(int i=0; i<10; i++){                                  /// has to be sent a few times to set com speed
    if(gsmModem.atPrint("AT\r","OK",500) == 0){
      break;
    }
  }
  
  gsmModem.atPrint("AT+CGNSPWR=1\r","OK");
  int res = gsmModem.atPrint("AT+CGNSINF\r","+CGNSINF: 1,1",5000,"+CGNSINF: 1,0");
  if(res == -1){
    Serial.println("GNSS| PWR on, No fix.");
  }
  else if(res == -2 || res == -3){
    Serial.println("GNSS| No fix, unknown.");
  }
  Serial.println(gsmModem.getInBuffer());
  /// parse gnss info string
  char* gnssInfoStr = gsmModem.getInBuffer();
  for(int i=0; commaCount < 19; i++){
    if(gnssInfoStr[i] == '\0'){ /// reached end of str.
      break;
    }
    if(gnssInfoStr[i] == ','){
      switch(commaCount){
        case 3:
          getSubstring(latitudeStr, gnssInfoStr, (prevCommaIndex + 1), i);
          Serial.print("GNSS| latitude: ");
          Serial.println(latitudeStr);
        break;
        case 4:
          getSubstring(longitudeStr, gnssInfoStr, (prevCommaIndex + 1), i);
          Serial.print("GNSS| longitude: ");
          Serial.println(longitudeStr);
        break;
        case 5:
          getSubstring(gnssHeightStr, gnssInfoStr, (prevCommaIndex + 1), i);
          Serial.print("GNSS| height: ");
          Serial.println(gnssHeightStr);
    /// TODO: clear trailing zeros from str
        break;
        case 14:
          getSubstring(gnssSateliteNumStr, gnssInfoStr, (prevCommaIndex + 1), i);
          Serial.print("GNSS| satNum: ");
          Serial.println(gnssSateliteNumStr);
        break;
        case 18:
          getSubstring(gnssCN0Str, gnssInfoStr, (prevCommaIndex + 1), i);
          Serial.print("GNSS| CN0: ");
          Serial.println(gnssCN0Str);
        break;
      }
      prevCommaIndex = i;
      commaCount++;
    }
  }
  for(int i=0; i<10; i++){                                  /// has to be sent a few times to set com speed
    if(gsmModem.atPrint("AT\r","OK",500) == 0){
      break;
    }
  }
  if(gsmModem.atPrint("AT+CSQ\r","OK") == 0){
    char* csqRespStr = gsmModem.getInBuffer();
    int startIndex = (strstr(csqRespStr, "+CSQ: ") + 6) - csqRespStr;
    int endIndex = strstr(csqRespStr, ",") - csqRespStr;
    getSubstring(rssiStr, csqRespStr, startIndex, endIndex);
    Serial.print("RSSI| ");
    Serial.println(rssiStr);
  }

  #define _MQTT_SIMINFO_LEN 200
  #define _TEMP_STRLEN 100
  char mqttSimInfoStr[_MQTT_SIMINFO_LEN] = "";
  char tempStrBuffer[_TEMP_STRLEN];

  if(latitudeStr != ""){
    snprintf(tempStrBuffer, _TEMP_STRLEN," \"latitude\":%s,",latitudeStr);
    strcat(mqttSimInfoStr, tempStrBuffer);
  }
  if(longitudeStr != ""){
    snprintf(tempStrBuffer, _TEMP_STRLEN," \"longitude\":%s,",longitudeStr);
    strcat(mqttSimInfoStr, tempStrBuffer);
  }
  if(gnssHeightStr != ""){
    snprintf(tempStrBuffer, _TEMP_STRLEN," \"masl\":%s,",gnssHeightStr);
    strcat(mqttSimInfoStr, tempStrBuffer);
  }
  if(gnssSateliteNumStr != ""){
    snprintf(tempStrBuffer, _TEMP_STRLEN," \"sat_no\":%s,",gnssSateliteNumStr);
    strcat(mqttSimInfoStr, tempStrBuffer);
  }
  if(gnssCN0Str != ""){
    snprintf(tempStrBuffer, _TEMP_STRLEN," \"cn0\":%s,",gnssCN0Str);
    strcat(mqttSimInfoStr, tempStrBuffer);
  }
  if(rssiStr != ""){
    snprintf(tempStrBuffer, _TEMP_STRLEN," \"rssi\":%s,",rssiStr);
    strcat(mqttSimInfoStr, tempStrBuffer);
  }

  strcat(mqttSimInfoStr, " \"gps_accuracy\": 1.2,");  /// const tags
  strcat(mqttSimInfoStr, " \"con_type\": \"EGPRS\",");  /// const tags
  
  //Serial.println(mqttSimInfoStr); 
  return mqttSimInfoStr;
}
/*void constructMqttMsq(){
  

  
  strcat(mqttPayloadBuffer, tempStrBuffer);
  strcat(mqttPayloadBuffer, " }");

  Serial.print(mqttPayloadBuffer);
  
}*/

void sendMqtt(){
  char mqttPayloadBuffer[500] = "{";
  char tempStrBuffer[100];
  unsigned int len = 5;
  //gsmModem.setMqttPayload(str,len);
  int i = 0;

  /// Collect data and create mqtt frame
 
  //constructMqttMsq();
  strcat(mqttPayloadBuffer,getGNSSaGSMinfo().c_str());
  snprintf(tempStrBuffer, sizeof(tempStrBuffer)/sizeof(char), " \"MSG_ID\":%d", (int)random(0,255));
  strcat(mqttPayloadBuffer, tempStrBuffer);
  strcat(mqttPayloadBuffer, " }");

  Serial.println(mqttPayloadBuffer);
  
  for(i=0; i<10; i++){                                  /// has to be sent a few times to set com speed
    if(gsmModem.atPrint("AT\r","OK",500) == 0){
      break;
    }
  }
  if(i==5 && gsmModem.atPrint("AT\r","OK",500) == -3){
    Serial.println("[ ER ] modem is not responding to AT");
    return;
  } 
  if(gsmModem.atPrint("AT+CNACT?\r","OK", 5000, "0.0.0.0") == -1){  /// check if connected already

    gsmModem.atPrint("ATE1\r","OK");
    gsmModem.atPrint("AT+CPIN?\r","OK");
    
    gsmModem.atPrint("AT+CNMP=13\r","OK");
    gsmModem.atPrint("AT+CNACTCFG=IPV4V6\r","OK", 10000);
    gsmModem.atPrint("AT+CREG?\r","+CREG: 0,1", 30000);
    gsmModem.atPrint("AT+CSQ\r","OK");
    
    gsmModem.atPrint("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r","OK");
    gsmModem.atPrint("AT+SAPBR=3,1,\"APN\",\"mts\"\r","OK");
    gsmModem.atPrint("AT+SAPBR=3,1,\"PWD\",\"064\"\r","OK");
  
    gsmModem.atPrint("AT+CGATT?\r", "+CGATT: 1");
    gsmModem.atPrint("AT+CNACT=1,\"mts\"\r","OK");///AT+CNACT=1,\"mts\"
  }
  //gsmModem.atPrint("AT\r","OK");
  //gsmModem.atPrint("AT+SAPBR?\r","+CREG: 0,1", 30000);

  if(gsmModem.atPrint("AT+SMCONF?\r","sandorr.eunetddns.net") != 0){ /// List Mqtt Settings, if URL matches, params are loaded into config from previous loop, do not config again
    gsmModem.atPrint("AT+SMCONF=\"URL\",sandorr.eunetddns.net,1883\r","OK"); /// List Mqtt Settings
    gsmModem.atPrint("AT+SMCONF=\"CLIENTID\",\"ESP_DEV0\"\r","OK"); /// List Mqtt Settings
    gsmModem.atPrint("AT+SMCONF=\"KEEPTIME\",60\r","OK"); /// List Mqtt Settings
  }
  gsmModem.atPrint("AT+SMCONN\r","OK"); /// List Mqtt Settings

  char mqttTopic[] = "RTU0/RTU_INFO";
  char mqttMsgBuffer[200];
  char mqttTopicATCMD[100];

  
  
  snprintf(mqttMsgBuffer, sizeof(mqttMsgBuffer)/sizeof(char), "%s\r", mqttPayloadBuffer); ///TODO: eliminate this, add /r in previous strcat
  snprintf(mqttTopicATCMD, sizeof(mqttTopicATCMD)/sizeof(char), "AT+SMPUB=\"%s\",\"%d\",1,1\r", mqttTopic, strlen(mqttMsgBuffer));
  
  Serial.println(mqttTopicATCMD);
  Serial.println(mqttMsgBuffer);
  gsmModem.atPrint(mqttTopicATCMD,">"); /// List Mqtt Settings
  gsmModem.atPrint(mqttMsgBuffer,"OK"); /// List Mqtt Settings
  gsmModem.atPrint("AT+SMDISC\r","OK"); /// List Mqtt Settings
 
 /*HSS burgije
 fi burgije:              12 10 8 6 5.5 5 4.5 4 3.5 3
 min kom. potrebno:       0   0 0 1 1   1 1   1 2   2
 dodatno ako nam daju:    1   1 1 1 1   2 2   2 0   2

 imbus                    1.5 2 2.5 3 4 4.5
 min kom. potrebno:       1   3   3 3 1   0
 dodatno ako nam daju:    1   2   2 2 1   1*/

 /*3.6x200
 2.5x150*/
}
void setup()  
{
  //simModem.begin(15200, SERIAL_8N1, 16, 17); 
  Serial.begin(115200);
  Serial.println("hello serial");
  gsmModem.init();
  Serial.println("GSM COM DUMP:");
  sendMqtt(); 
} 
void loop()  
{ 
  
  delay(10000);
  Serial.println("loop");
}

int sendATCommand(const char* command, const char* response ){
  

}
