void getSubstring(char* destination, char* source, uint8_t startIndex, uint8_t endIndex) {
  int j = 0;
  for (int i = startIndex; i < endIndex; i++) {
    destination[j++] = source[i];
  }
}

void clearTrailingZeros(char* string) {
  for (int i = 0; i < strlen(string); i++) {
    //DebugSerial.println(*(string + strlen(string) - i));
    if (strstr(string, ".") == NULL) {
      return;
    }
  }
}
void clearDecimals(char* string) {
  char* s = strstr(string, ".");
  if (s == NULL) {
    return;
  }
  *s = '\0';                          /// !! this will segfault if cstr was created using char* instead of char[]
}

void getSimformation(char (& mqttPayloadBuff)[MQTT_PAYLOAD_BUFF_LEN], char (& tempStrBuffer)[_TEMP_STRLEN]) {

  if(0 != simModem.simformation.gnss_latitude){
    snprintf(tempStrBuffer, _TEMP_STRLEN, " \"latitude\":%.6lf,", simModem.simformation.gnss_latitude);
    strcat(mqttPayloadBuff, tempStrBuffer);
  }

  if(0 != simModem.simformation.gnss_longitude){
    snprintf(tempStrBuffer, _TEMP_STRLEN, " \"longitude\":%.6lf,", simModem.simformation.gnss_longitude);
    strcat(mqttPayloadBuff, tempStrBuffer);
  }

  if(0 != simModem.simformation.gnss_masl){
    snprintf(tempStrBuffer, _TEMP_STRLEN, " \"masl\":%.2lf,", simModem.simformation.gnss_masl);
    strcat(mqttPayloadBuff, tempStrBuffer);
  }

  if(0 != simModem.simformation.gnss_satelites){
    snprintf(tempStrBuffer, _TEMP_STRLEN, " \"sat_no\":%d,", simModem.simformation.gnss_satelites);
    strcat(mqttPayloadBuff, tempStrBuffer);
  }

  if(0 != simModem.simformation.gnss_cnoMax){
    snprintf(tempStrBuffer, _TEMP_STRLEN, " \"cn0\":%d,", simModem.simformation.gnss_cnoMax);
    strcat(mqttPayloadBuff, tempStrBuffer);
  }

  if(0 != simModem.simformation.sim_rssi){
    snprintf(tempStrBuffer, _TEMP_STRLEN, " \"rssi\":%d,", simModem.simformation.sim_rssi);
    strcat(mqttPayloadBuff, tempStrBuffer);
  }

  /*if(WiFi.status() == WL_CONNECTED){
    snprintf(tempStrBuffer, _TEMP_STRLEN, " \"con_type\":\"WIFI\",");
    strcat(mqttPayloadBuff, tempStrBuffer);
  }
  else{
    snprintf(tempStrBuffer, _TEMP_STRLEN, " \"con_type\":\"EGPRS\",");
    strcat(mqttPayloadBuff, tempStrBuffer);
  }*/

}
//void getGNSSaGSMinfo(SIM7000& gsmModem, char (& mqttPayloadBuff)[MQTT_PAYLOAD_BUFF_LEN], char (& tempStrBuffer)[_TEMP_STRLEN]){
//
//  char latitudeStr[11] = "";
//  char longitudeStr[12] = "";
//  char gnssHeightStr[9] = "";
//  char gnssSateliteNumStr[5] = "";
//  char gnssCN0Str[5] = "";
//  char rssiStr[6] = "";
//
//  uint8_t commaCount = 0;
//  uint8_t prevCommaIndex = 0;
//
//  #define _GNSS_GSM_VERBOSE 1
//
//  //wakeUpAT(gsmModem);
//
//  /// GET RSSI INFO
//
//    /*  <rssi>
//    0 - 115 dBm or less
//    1 - 111 dBm
//    2...30 - 110... - 54 dBm
//    31 - 52 dBm or greater
//    99 not known or not detectable*/
//
//  if(gsmModem.atSend("AT+CSQ\r","OK") == 0){
//    const int RSSILUT[] = {115, 111, 110, 109, 107, 105, 103, 101, 99, 97, 95, 93, 91, 89, 87, 85, 83, 81, 79, 77, 75, 73, 71, 69, 67, 65, 63, 61, 59, 57, 55, 53};
//    char* csqRespStr = gsmModem.getInBuffer();
//    int startIndex = (strstr(csqRespStr, "+CSQ: ") + 6) - csqRespStr;
//    int endIndex = strstr(csqRespStr, ",") - csqRespStr;
//    getSubstring(rssiStr, csqRespStr, startIndex, endIndex);
//    int rssiSimNotation = atoi(rssiStr);
//
//    if(rssiSimNotation < 32){                       /// convert SIM response to dBm
//      rssiSimNotation = RSSILUT[rssiSimNotation];
//    }
//    else{
//      rssiSimNotation = 0;  /// 99 means unknown or no connection
//    }
//    snprintf(rssiStr, 4,"-%d",rssiSimNotation);
//
//
// //gsmModem.atPrint("AT+CUSD=1\r","OK");
// //gsmModem.atPrint("AT+CUSD=1,\"*100#\",15\r","OK");
//
//
//#ifdef _GNSS_GSM_VERBOSE
//    DebugSerial.print("RSSI| ");
//    DebugSerial.println(rssiStr);
//    DebugSerial.print("RSSI dBm| ");
//    DebugSerial.print(rssiSimNotation);
//#endif
//  }
//
//  /// GET GNSS INFO
//  gsmModem.atSend("AT+CGNSPWR=1\r","OK");
//  int res = gsmModem.atSend("AT+CGNSINF\r","+CGNSINF: 1,1",5000,"+CGNSINF: 1,0");
//
//#ifdef _GNSS_GSM_VERBOSE
//  if(res == -1){DebugSerial.println("GNSS| PWR on, No fix.");}
//  else if(res == -2 || res == -3){DebugSerial.println("GNSS| No fix, unknown.");}
//  DebugSerial.println(gsmModem.getInBuffer());
//#endif
//
//  /// parse gnss info string
//  char* gnssInfoStr = gsmModem.getInBuffer();
//  for(int i=0; commaCount < 19; i++){
//    if(gnssInfoStr[i] == '\0'){ /// reached end of str.
//      break;
//    }
//    if(gnssInfoStr[i] == ','){
//      switch(commaCount){
//        case 3:
//          getSubstring(latitudeStr, gnssInfoStr, (prevCommaIndex + 1), i);
//        break;
//        case 4:
//          getSubstring(longitudeStr, gnssInfoStr, (prevCommaIndex + 1), i);
//        break;
//        case 5:
//          getSubstring(gnssHeightStr, gnssInfoStr, (prevCommaIndex + 1), i);
//         // clearTrailingZeros(gnssHeightStr); ///clear trailing zeros from str
//          clearDecimals(gnssHeightStr);
//        break;
//        case 14:
//          getSubstring(gnssSateliteNumStr, gnssInfoStr, (prevCommaIndex + 1), i);
//        break;
//        case 18:
//          getSubstring(gnssCN0Str, gnssInfoStr, (prevCommaIndex + 1), i);
//        break;
//      }
//#ifdef _GNSS_GSM_VERBOSE
//      switch(commaCount){
//        case 3:
//          DebugSerial.print("GNSS| latitude: ");
//          DebugSerial.println(latitudeStr);
//        break;
//        case 4:
//          DebugSerial.print("GNSS| longitude: ");
//          DebugSerial.println(longitudeStr);
//        break;
//        case 5:
//          DebugSerial.print("GNSS| height: ");
//          DebugSerial.println(gnssHeightStr);/// TODO: clear trailing zeros from str
//        break;
//        case 14:
//          DebugSerial.print("GNSS| satNum: ");
//          DebugSerial.println(gnssSateliteNumStr);
//        break;
//        case 18:
//          DebugSerial.print("GNSS| CN0: ");
//          DebugSerial.println(gnssCN0Str);
//        break;
//      }
//#endif
//      prevCommaIndex = i;
//      commaCount++;
//    }
//  }
//
//  /// Put everything into mqtt buffer
//
//  if(latitudeStr[0] != '\0' && longitudeStr[0] != '\0'){                        /// append latitude, longitude, (height) and gps accuracy
//    snprintf(tempStrBuffer, _TEMP_STRLEN," \"latitude\":%s,",latitudeStr);
//    strcat(mqttPayloadBuff, tempStrBuffer);
//    snprintf(tempStrBuffer, _TEMP_STRLEN," \"longitude\":%s,",longitudeStr);
//    strcat(mqttPayloadBuff, tempStrBuffer);
//    if(gnssHeightStr[0] != '\0'){
//      snprintf(tempStrBuffer, _TEMP_STRLEN," \"masl\":%s,",gnssHeightStr);
//      strcat(mqttPayloadBuff, tempStrBuffer);
//    }
//    //strcat(mqttPayloadBuff, " \"gps_accuracy\": 1.2,");  /// const tags
//  }
//  if(gnssSateliteNumStr[0] != '\0'){                                            /// num of sats, even when no lock
//    snprintf(tempStrBuffer, _TEMP_STRLEN," \"sat_no\":%s,",gnssSateliteNumStr);
//    strcat(mqttPayloadBuff, tempStrBuffer);
//  }
//  if(gnssCN0Str[0] != '\0'){                                                    /// and signal quality
//    snprintf(tempStrBuffer, _TEMP_STRLEN," \"cn0\":%s,",gnssCN0Str);
//    strcat(mqttPayloadBuff, tempStrBuffer);
//  }
//  if(rssiStr[0] != '\0'){                                                       /// and rssi
//    snprintf(tempStrBuffer, _TEMP_STRLEN," \"rssi\":%s,",rssiStr);
//    strcat(mqttPayloadBuff, tempStrBuffer);
//  }
//  //strcat(mqttPayloadBuff, " \"con_type\":\"EGPRS\",");                         /// add comm link type
//
//  /// return - mqttPayloadBuff modified, data added!
//}
