/**
 * Takes:
 *  - ref. to SIM7000 object
 *  - ref. to mqtt Str buffer which is of defined length, buffer should be empty, but TODO: check if it's empty
 *  - char* to mqttTopic string
 * Returns:
 *  - 0 or -1 as indicator of success or fail
 * function sends msg contained in mqttPayloadBuff to mqtt server using SIM7000G module using sim7000.h functions
 * ! uses char mqttPayloadBuffer[500] = "{"; from global scope
 */
int sendMqttSIM(SIM7000& gsmModem,char(& mqttPayloadBuff)[500], const char* mqttTopic){
  //char mqttPayloadBuffer[500] = "{";
  char tempStrBuffer[_TEMP_STRLEN];
  //char mqttTopic[] = "RTU0/RTU_INFO";
  char mqttTopicATCMD[100];
  unsigned int len = 5;
  int i = 0;

  for(i=0; i<10; i++){                                  /// has to be sent a few times to set com speed /// @TODO: this has it's own function in sim7000.h if I remember correctly.
    if(gsmModem.atPrint("AT\r","OK",500) == 0){
      break;
    }
  }
  if(i>9 && gsmModem.atPrint("AT\r","OK",500) == -3){
    Serial.println("[ ER ] modem is not responding to AT");
    return -1;
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
  return 0;
 
}

/**
 * Takes:
 *  - ref. to mqtt Str buffer which is of defined length, buffer should be empty, but TODO: check if it's empty
 *  - char* to mqttTopic string
 * Returns:
 *  - 0 or -1 as indicator of success or fail
 * function sends msg contained in mqttPayloadBuff to mqtt server using the ESP32 Wifi device and PubSubClient library
 * ! uses char mqttPayloadBuffer[500] = "{"; from global scope
 */
int sendMqttWLAN(char(& mqttPayloadBuff)[500], const char* mqttTopic){

  #define MQTT_CHECKIF_CONNECTED 200
  #define MQTT_CONNECT_TIMEOUT 30000

  if(isWifiAvailable(30000) != 0){
    return -1;
  }
  const char* mqttServer = "sandorr.eunetddns.net";
  const char* mqttClientName = "ESP32Client";
  const int mqttPort = 1883;
  
  WiFiClient espClient;
  PubSubClient client(espClient);

  client.setServer(mqttServer, mqttPort);

  Serial.println(F("WLAN-MQTT| Connecting to broker."));
  for(int i=0; !client.connected(); i++){
    client.connect(mqttClientName);             /// mqttUser, mqttPassword 
    delay(MQTT_CHECKIF_CONNECTED);
    if(i > (MQTT_CONNECT_TIMEOUT/MQTT_CHECKIF_CONNECTED)){
      Serial.println(F("WLAN-MQTT|[ER] Can't connect to server- timeout."));
      Serial.print(F("WLAN-MQTT| failed with status: "));
      Serial.println(client.state());
      return -1;
    }
  }
  Serial.println(F("WLAN-MQTT| Connected."));
  Serial.print(F("WLAN-MQTT| Sending: "));
  Serial.println(mqttPayloadBuff);
  int res = client.publish(mqttTopic, mqttPayloadBuff,strlen(mqttPayloadBuff)); //
  if (res == 0) {                                       /// pubsub returns false if write is not successfull.
    Serial.println(F("WLAN-MQTT|[ER] falied to send mqtt msg!"));
    return -1;
  }
  Serial.println(F("WLAN-MQTT|[OK] msg sent!"));
  return 0;
}

/**
 * Takes:
 *  - ref. to SIM7000 object
 *  - ref. to mqtt Str buffer which is of defined length, buffer should be empty, but TODO: check if it's empty
 *  - char* to mqttTopic string
 * Returns:
 *  - 0 or -1 as indicator of success or fail
 * function is a wrapper, that tryes to send mqtt via primary mode of transmission, if that fails tryes backup mode to send mqtt.
 * @TODO: This is not a great way of defining primary and secondary com devices...
 * ! uses char mqttPayloadBuffer[500] = "{"; from global scope
 */
void sendMqtt(SIM7000& gsmModem,char(& mqttPayloadBuff)[500], const char* mqttTopic){
  #define MAIN_COM sendMqttWLAN(mqttPayloadBuff,mqttTopic)  /// Main mode of communication - wifi is prefered.
  #define BACKUP_COM sendMqttSIM(gsmModem,mqttPayloadBuff,mqttTopic)
  if(MAIN_COM == 0){
    return;
  }
  BACKUP_COM;
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
