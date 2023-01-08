void sendMqtt(SIM7000& gsmModem,char(& mqttPayloadBuff)[500], const char* mqttTopic){
  //char mqttPayloadBuffer[500] = "{";
  char tempStrBuffer[_TEMP_STRLEN];
  //char mqttTopic[] = "RTU0/RTU_INFO";
  char mqttTopicATCMD[100];
  unsigned int len = 5;
  int i = 0;

  /// Collect data and create mqtt frame
 // readVoltages();
 // addVoltageMqttTags(mqttPayloadBuffer, tempStrBuffer);
 // getGNSSaGSMinfo(gsmModem, mqttPayloadBuffer, tempStrBuffer);
  
//  snprintf(tempStrBuffer, sizeof(tempStrBuffer)/sizeof(char), " \"msg_id\":%d", (int)random(0,255));
 // strcat(mqttPayloadBuffer, tempStrBuffer);
//  strcat(mqttPayloadBuffer, " }\r");                                                                    /// !! the \r is very important, plese do not delete
 // Serial.println(mqttPayloadBuffer);

 // return;
  
  for(i=0; i<10; i++){                                  /// has to be sent a few times to set com speed
    if(gsmModem.atPrint("AT\r","OK",500) == 0){
      break;
    }
  }
  if(i>9 && gsmModem.atPrint("AT\r","OK",500) == -3){
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
  if(gsmModem.atPrint("AT+SMCONN\r","OK", 10000, "ERROR") != 0){
    Serial.println("MQTT| probably not connected-server might be offline. ");
  }

  snprintf(mqttTopicATCMD, sizeof(mqttTopicATCMD)/sizeof(char), "AT+SMPUB=\"%s\",\"%d\",1,1\r", mqttTopic, strlen(mqttPayloadBuff));

  Serial.print("MQTT| topic:");
  Serial.println(mqttTopicATCMD);
  Serial.print("MQTT| sending:");
  Serial.println(mqttPayloadBuff);

  gsmModem.atPrint(mqttTopicATCMD,">");
  if(gsmModem.atPrint(mqttPayloadBuff,"OK") == 0){
    Serial.println("MQTT| sent. [ OK ]");
  } 
  else{
    Serial.println("MQTT| [ Error ]");
  }
  gsmModem.atPrint("AT+SMDISC\r","OK"); 
 // gsmModem.atPrint("AT+SMDISC\r","OK");
 
}
/**
 * Takes:
 *  - ref. to SIM7000 object
 *  - ref. to mqtt Str buffer which is of defined length, buffer should be empty, but TODO: check if it's empty
 *  - array of pointers to 'BasicHiveSensor's
 *  - number of p in pArray
 * Returns:
 *  -*- modifies string in mqttPayloadBuff
 *  
 * function gathers data from SIM9000 module, and reads Battery voltages + adds 'msg_id' to help keep track of msges
 * 
 * ! uses char mqttPayloadBuffer[500] = "{"; from global scope
 */
void sendMqttStatusMsg(SIM7000& gsmModem,char(& mqttPayloadBuff)[500], const char* topicName){
  #define _TEMP_STRLEN 100
  char tempStrBuffer[_TEMP_STRLEN];
  static uint8_t msgId = 0;

  msgId++;
  mqttPayloadBuff[0] = '\0';
  strcat(mqttPayloadBuff, "{"); 
  /// Collect data and create mqtt frame
  readVoltages();
  addVoltageMqttTags(mqttPayloadBuff, tempStrBuffer);
  getGNSSaGSMinfo(gsmModem, mqttPayloadBuff, tempStrBuffer);
  
  snprintf(tempStrBuffer, sizeof(tempStrBuffer)/sizeof(char), " \"msg_id\":%d", (int)msgId);
  strcat(mqttPayloadBuff, tempStrBuffer);
  strcat(mqttPayloadBuff, " }\r");                                                                    /// !! the \r is very important, plese do not delete
  Serial.println(mqttPayloadBuff);

  /// TODO: should call mqtt send from here...
  sendMqtt(gsmModem,mqttPayloadBuff,topicName);
}

/**
 * Takes:
 *  - ref. to SIM7000 object
 *  - ref. to mqtt Str buffer which is of defined length, buffer should be empty, but TODO: check if it's empty
 *  - array of pointers to 'BasicHiveSensor's
 *  - number of p in pArray
 * Returns:
 *  -*- modifies string in mqttPayloadBuff
 *  
 * function gathers data from 'BasicHiveSensor' sensors and puts them into mqtt buffer
 * 
 * ! uses char mqttPayloadBuffer[500] = "{"; from global scope
 */
void sendMqttBHSensorMsg(SIM7000& gsmModem,char(& mqttPayloadBuff)[500],const char* topicName, BasicHiveSensor* pBHSensors[], uint8_t numOfBHSensors){
  #define _TEMP_STRLEN 100
  char tempStrBuffer[_TEMP_STRLEN];
  static uint8_t msgId = 0;
  msgId++;
  mqttPayloadBuff[0] = '\0';
  strcat(mqttPayloadBuff, "{"); 
  /// Collect data and create mqtt frame
  for(int i=0; i<numOfBHSensors; i++){
    pBHSensors[i]->addMqttTags(mqttPayloadBuff, tempStrBuffer);
  }

  snprintf(tempStrBuffer, sizeof(tempStrBuffer)/sizeof(char), " \"msg_id\":%d", (int)msgId);
  strcat(mqttPayloadBuff, tempStrBuffer);
  strcat(mqttPayloadBuff, " }\r");                                                                    /// !! the \r is very important, plese do not delete
  Serial.println(mqttPayloadBuff);

  /// TODO: should call mqtt send from here...
  sendMqtt(gsmModem,mqttPayloadBuff, topicName);
}
