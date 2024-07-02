///**
// * Takes:
// *  - ref. to SIM7000 object
// *  - ref. to mqtt Str buffer which is of defined length, buffer should be empty, but TODO: check if it's empty
// *  - char* to mqttTopic string
// * Returns:
// *  - 0 or -1 as indicator of success or fail
// * function sends msg contained in mqttPayloadBuff to mqtt server using SIM7000G module using sim7000.h functions
// * ! uses char mqttPayloadBuffer[500] = "{"; from global scope
// */
//int sendMqttSIM(SIM7000& gsmModem,char(& mqttPayloadBuff)[MQTT_PAYLOAD_BUFF_LEN], const char* mqttTopic){
//  //char mqttPayloadBuffer[500] = "{";
//  char tempStrBuffer[_TEMP_STRLEN];
//  //char mqttTopic[] = "RTU0/RTU_INFO";
//  char mqttTopicATCMD[100];
//  unsigned int len = 5;
//  int i = 0;
//
//  /*for(i=0; i<10; i++){                                  /// has to be sent a few times to set com speed /// @TODO: this has it's own function in sim7000.h if I remember correctly.
//    if(gsmModem.atSend("AT\r","OK",500) == 0){
//      break;
//    }
//  }
//  if(i>9 && gsmModem.atSend("AT\r","OK",500) == -3){
//    DebugSerial.println("[ ER ] modem is not responding to AT");
//    return -1;
//  } */
//  
//  if(gsmModem.atSend("AT+CNACT?\r","OK", 5000, "0.0.0.0") == -5){  /// check if connected already
//
//    gsmModem.atSend("ATE1\r","OK");
//    gsmModem.atSend("AT+CPIN?\r","OK");
//    
//    gsmModem.atSend("AT+CNMP=13\r","OK");
//    gsmModem.atSend("AT+CNACTCFG=IPV4V6\r","OK", 10000);
//    gsmModem.atSend("AT+CREG?\r","+CREG: 0,1", 30000);
//    gsmModem.atSend("AT+CSQ\r","OK");
//    
//    gsmModem.atSend("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r","OK");
//    gsmModem.atSend("AT+SAPBR=3,1,\"APN\",\"mts\"\r","OK");
//    gsmModem.atSend("AT+SAPBR=3,1,\"PWD\",\"064\"\r","OK");
//  
//    gsmModem.atSend("AT+CGATT?\r", "+CGATT: 1");
//    gsmModem.atSend("AT+CNACT=1,\"mts\"\r","OK");///AT+CNACT=1,\"mts\"
//  }
//
// /* if(gsmModem.atSend("AT+CNACT?\r","OK", 5000, "0.0.0.0") != 0){
//     powerKeyCycle = true;
//  }*/
//  //gsmModem.atSend("AT\r","OK");
//  //gsmModem.atSend("AT+SAPBR?\r","+CREG: 0,1", 30000);
//
//  if(gsmModem.atSend("AT+SMCONF?\r","fflori4nhahomelab.duckdns.org/") != 0){ /// List Mqtt Settings, if URL matches, params are loaded into config from previous loop, do not config again
//    gsmModem.atSend("AT+SMCONF=\"URL\",fflori4nhahomelab.duckdns.org,1883\r","OK"); /// List Mqtt Settings
//    gsmModem.atSend("AT+SMCONF=\"CLIENTID\",\"ESP_DEV0\"\r","OK"); /// List Mqtt Settings
//    gsmModem.atSend("AT+SMCONF=\"KEEPTIME\",60\r","OK"); /// List Mqtt Settings
//  }
//  if(gsmModem.atSend("AT+SMCONN\r","OK", 10000, "ERROR") != 0){
//    DebugSerial.println("MQTT| probably not connected-server might be offline. ");
//  }
//
//  snprintf(mqttTopicATCMD, sizeof(mqttTopicATCMD)/sizeof(char), "AT+SMPUB=\"%s\",\"%d\",1,1\r", mqttTopic, strlen(mqttPayloadBuff));
//
//  DebugSerial.print("MQTT| topic:");
//  DebugSerial.println(mqttTopicATCMD);
//  DebugSerial.print("MQTT| sending:");
//  DebugSerial.println(mqttPayloadBuff);
//
//  gsmModem.atSend(mqttTopicATCMD,">");
//  if(gsmModem.atSend(mqttPayloadBuff,"OK") == 0){
//    DebugSerial.println("MQTT| sent. [ OK ]");
//  } 
//  else{
//    DebugSerial.println("MQTT| [ Error ]");  
//  }
//  gsmModem.atSend("AT+SMDISC\r","OK"); 
// // gsmModem.atSend("AT+SMDISC\r","OK");
//
// if(atNoResponseCounter > 100){
//   powerKeyCycle = true;
// }
//  return 0;
// 
//}

/**
 * Takes:
 *  - ref. to mqtt Str buffer which is of defined length, buffer should be empty, but TODO: check if it's empty
 *  - char* to mqttTopic string
 * Returns:
 *  - 0 or -1 as indicator of success or fail
 * function sends msg contained in mqttPayloadBuff to mqtt server using the ESP32 Wifi device and PubSubClient library
 * ! uses char mqttPayloadBuffer[500] = "{"; from global scope
 */
int sendMqttWLAN(char(& mqttPayloadBuff)[MQTT_PAYLOAD_BUFF_LEN], const char* mqttTopic){

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

  DebugSerial.println(F("WLAN-MQTT| Connecting to broker."));
  for(int i=0; !client.connected(); i++){
    client.connect(mqttClientName);             /// mqttUser, mqttPassword 
    delay(MQTT_CHECKIF_CONNECTED);
    if(i > (MQTT_CONNECT_TIMEOUT/MQTT_CHECKIF_CONNECTED)){
      DebugSerial.println(F("WLAN-MQTT|[ER] Can't connect to server- timeout."));
      DebugSerial.print(F("WLAN-MQTT| failed with status: "));
      DebugSerial.println(client.state());
      return -1;
    }
  }
  DebugSerial.println(F("WLAN-MQTT| Connected."));
  DebugSerial.print(F("WLAN-MQTT| Sending: "));
  DebugSerial.println(mqttPayloadBuff);
  int res = client.publish(mqttTopic, mqttPayloadBuff,strlen(mqttPayloadBuff)); //
  if (res == 0) {                                       /// pubsub returns false if write is not successfull.
    DebugSerial.println(F("WLAN-MQTT|[ER] falied to send mqtt msg!"));
    return -1;
  }
  DebugSerial.println(F("WLAN-MQTT|[OK] msg sent!"));
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
void sendMqtt(char(& mqttPayloadBuff)[MQTT_PAYLOAD_BUFF_LEN], const char* mqttTopic){
  /*#define MAIN_COM */
  int8_t res = simModem.sendMqtt(mqttPayloadBuff, mqttTopic);/*sendMqttSIM(gsmModem,mqttPayloadBuff,mqttTopic)*/
  if(res == 0){
    esp_task_wdt_reset();
    mqttMsgId++;
  }
 /* #define BACKUP_COM sendMqttWLAN(mqttPayloadBuff,mqttTopic)  /// Main mode of communication - wifi is prefered.
  if(MAIN_COM == 0){
    return;
  }*/
 // BACKUP_COM;
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
void sendMqttStatusMsg(char(& mqttPayloadBuff)[MQTT_PAYLOAD_BUFF_LEN], const char* topicName){
  char tempStrBuffer[_TEMP_STRLEN];
 
  mqttPayloadBuff[0] = '\0';
  strcat(mqttPayloadBuff, "{"); 
  /// Collect data and create mqtt frame
  addVoltageMqttTags(mqttPayloadBuff, tempStrBuffer);
  addSolLoggerMqttTags(mqttPayloadBuff, tempStrBuffer);
  addESPResetStsMqttTags(mqttPayloadBuff, tempStrBuffer);
  getSimformation(mqttPayloadBuff, tempStrBuffer);
  
  snprintf(tempStrBuffer, sizeof(tempStrBuffer)/sizeof(char), " \"msg_id\":%d", mqttMsgId);
  strcat(mqttPayloadBuff, tempStrBuffer);
  strcat(mqttPayloadBuff, " }\r");                                                                    /// !! the \r is very important, plese do not delete
  DebugSerial.println(mqttPayloadBuff);

  /// TODO: should call mqtt send from here...
  sendMqtt(mqttPayloadBuff,topicName);
  
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
void sendMqttBHSensorMsg(char(& mqttPayloadBuff)[MQTT_PAYLOAD_BUFF_LEN],const char* topicName, BasicHiveSensor* pBHSensors[], uint8_t numOfBHSensors){
  char tempStrBuffer[_TEMP_STRLEN];

  mqttPayloadBuff[0] = '\0';
  strcat(mqttPayloadBuff, "{"); 
  /// Collect data and create mqtt frame
  for(int i=0; i<numOfBHSensors; i++){
    pBHSensors[i]->addMqttTags(mqttPayloadBuff, tempStrBuffer);
  }

  snprintf(tempStrBuffer, sizeof(tempStrBuffer)/sizeof(char), " \"msg_id\":%d", (int)mqttMsgId);
  strcat(mqttPayloadBuff, tempStrBuffer);
  strcat(mqttPayloadBuff, " }\r");                                                                    /// !! the \r is very important, plese do not delete
  DebugSerial.println(mqttPayloadBuff);

  /// TODO: should call mqtt send from here...
  sendMqtt(mqttPayloadBuff, topicName);
}
