void sendHubStatusMqtt(SIM7000& gsmModem){
  char mqttPayloadBuffer[500] = "{";
  char tempStrBuffer[_TEMP_STRLEN];
  char mqttTopic[] = "RTU0/RTU_INFO";
  char mqttTopicATCMD[100];
  unsigned int len = 5;
  int i = 0;

  /// Collect data and create mqtt frame
  readVoltages();
  addVoltageMqttTags(mqttPayloadBuffer, tempStrBuffer);
  getGNSSaGSMinfo(gsmModem, mqttPayloadBuffer, tempStrBuffer);
  
  snprintf(tempStrBuffer, sizeof(tempStrBuffer)/sizeof(char), " \"MSG_ID\":%d", (int)random(0,255));
  strcat(mqttPayloadBuffer, tempStrBuffer);
  strcat(mqttPayloadBuffer, " }\r");                                                                    /// !! the \r is very important, plese do not delete
  Serial.println(mqttPayloadBuffer);
  
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
  if(gsmModem.atPrint("AT+SMCONN\r","OK", 30000, "ERROR") != 0){
    Serial.println("MQTT| probably not connected-server might be offline. ");
  }

  //snprintf(mqttMsgBuffer, sizeof(mqttMsgBuffer)/sizeof(char), "%s\r", mqttPayloadBuffer); ///TODO: eliminate this, add /r in previous strcat
  snprintf(mqttTopicATCMD, sizeof(mqttTopicATCMD)/sizeof(char), "AT+SMPUB=\"%s\",\"%d\",1,1\r", mqttTopic, strlen(mqttPayloadBuffer));

  Serial.print("MQTT| topic:");
  Serial.println(mqttTopicATCMD);
  Serial.print("MQTT| sending:");
  Serial.println(mqttPayloadBuffer);

  gsmModem.atPrint(mqttTopicATCMD,">");
  if(gsmModem.atPrint(mqttPayloadBuffer,"OK") == 0){
    Serial.println("MQTT| sent. [ OK ]");
  } 
  gsmModem.atPrint("AT+SMDISC\r","OK"); 
 
}
