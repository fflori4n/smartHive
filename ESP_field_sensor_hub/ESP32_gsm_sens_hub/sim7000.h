/*
    Functions for interfacing with a SIM 7000G chip, com uses hardware UART on the ESP32
    #include <HardwareSerial.h> is a dependency.
*/
#include <HardwareSerial.h>   /// Can't use two software serial instances?
//#include <SoftwareSerial.h>

HardwareSerial GSMModemSerial(2); // use UART2
//SoftwareSerial GSMModemSerial;

#define GSMBAUD 9600
#define GSMRX_PIN 16
#define GSMTX_PIN 17

#define GETMILISECS() (uint32_t)(int32_t)(esp_timer_get_time()/1000)  /// efficient? Yes. Cursed? Also yes.
#define GETELAPSEDMS() ((uint32_t)(int32_t)(esp_timer_get_time()/1000) - this->startedMs)

#define IN_BUFF_LEN 300
#define NMEA_BUFF_LEN 200

class SIM7000 {

  private:
    char serInBuff[IN_BUFF_LEN];
    char nMEARespBUff[NMEA_BUFF_LEN];

    uint32_t timeLimitms = 5000;
    uint32_t startedMs = 0;
    unsigned int buffIndex = 0;
    int atCmdRes = -3;      /// 0 -ok, -1 -error, -2 unknown if succeeded, -3 no response

    /*char* mqttPayload;
      unsigned int mqttPayloadLen = 0;*/

    int wakeUpAT() {    /// AT has to be sent a few times to set com speed
      int methodSig = -1;
      for (byte i = 0; i < 10; i++) {
        if (this->atPrint("AT\r", "OK", 500) == 0) {
          methodSig = 0;
          break;
        }
      }
      return methodSig;
    }

    int getDataStr(char* returnStr, byte tag, bool querry = false) {

#define RESP_KEY "+CGNSINF:"
#define RESP_KEY_LEN 10 ///sizeof(RESP_KEY)/sizeof(char)

      /*
         +CGNSINF: 1,1,20230327212102.000,46.103447,19.635575,132.200,0.00,356.7,1,,1.0,1.4,0.9,,22,7,1,,32,,
          1 - RUN STATUS
          2 - FIX STATUS
          3 - UTC TIME
          4 - LATITUDE
          5 - LONGITUDE
          6 - MASL METERS
          7 - GROUND SPEED
          8 - CURSE
          9 - FIX MODE
          10 - NOT USED
          11 - HDOP
          12 - PDOP
          13 - VDOP
          14 - RESERVED
          15 - GNSS Satellites in View
          16 - GPS Satellites Used
          17 - GLONASS Satellites used
          18 - Reserved3
          19 - C/N0 max
          20 - HPA
          21 - VPA
      */
      char* nMEAResponseString = this->nMEARespBUff;

      if (querry) {
        if (this->wakeUpAT() != 0) {
          return -1;
        }
        this->atPrint("AT+CGNSPWR=1\r", "OK");
        int res = this->atPrint("AT+CGNSINF\r", "+CGNSINF: 1,1", 5000, "+CGNSINF: 1,0");

#ifdef _GNSS_GSM_VERBOSE
        if (res == -1) {
          Serial.println("GNSS| PWR on, No fix.");
        }
        else if (res == -2 || res == -3) {
          Serial.println("GNSS| No fix, unknown.");
        }
#endif
        if (res != 0) {
          return -1;
        }
        if (strstr(this->getInBuffer(), RESP_KEY) == NULL) { /// key not found in buff
          return -1;
        }

        strncpy(nMEARespBUff, strstr(this->getInBuffer(), RESP_KEY) + RESP_KEY_LEN, NMEA_BUFF_LEN); //strncpy NOT NULL TERMINATING!!!
        //nMEAResponseString = strstr(nMEAResponseString, RESP_KEY) ;
        for (int i = (nMEAResponseString - nMEARespBUff); ((nMEARespBUff[i] != '\0') && (i < NMEA_BUFF_LEN)); i++) {
          if (nMEARespBUff[i] == '\r' || nMEARespBUff[i] == '\n') {
            nMEARespBUff[i] = '\0';
          }
        }
        Serial.println("Updated GNSS STR.: ");
        Serial.println(nMEAResponseString);

      }

      uint8_t comma = 0;
      char* startIndex = this->nMEARespBUff;//nMEARespBUff;
      char* endIndex = NULL;
      //char returnStr[20] = "";

      for (int i = 0; (i < NMEA_BUFF_LEN) && (nMEARespBUff[i] != '\0'); i++) {
        if (nMEARespBUff[i] == ',' || nMEARespBUff[i] == '\0') {
          comma++;
          if (comma == (tag - 1)) {
            startIndex = &(nMEARespBUff[i]) + 1;
          }
          else if (comma == tag) {
            endIndex = &(nMEARespBUff[i]);
            strncpy(returnStr, startIndex, (endIndex - startIndex)); //strncpy NOT NULL TERMINATING!!!
            returnStr[(endIndex - startIndex)] = '\0';
            Serial.println("PROC. RETURN STR: ");
            Serial.println(returnStr);
            return 0;
          }
        }
      }
    }


  public:

  enum tATResponse{
        aTresp_unknown = -1,
        aTresp_noResponse = -2,
        aTresp_timeout = -3,
        aTresp_noKeyFound = -4,
        aTresp_failKeyFound = -5,
        aTresp_ok = 0
      };

    SIM7000() {
      //GSMModemSerial.begin(GSMBAUD, SERIAL_8N1, GSMRX_PIN, GSMTX_PIN);     ///TODO: for some reason this does not get executed? Some C++ quirk probably, so moved to separate function fix me later
    }
    void init() {
      delay(1000);
      //GSMModemSerial.begin(9600, SWSERIAL_8N1, GSMRX_PIN, GSMTX_PIN, false);
      GSMModemSerial.begin(GSMBAUD, SERIAL_8N1, GSMRX_PIN, GSMTX_PIN);
      Serial.println("GSMModemSerial STARTED");
      delay(1000);
    }

    char* getInBuffer() {
      return this->serInBuff;
    }

    /*void setMqttPayload(char* mqttPaylodPtr, unsigned int len){
      this->mqttPayload = mqttPaylodPtr;
      this->mqttPayloadLen = len;
      }*/
    /*char* getMqttPayloadString(){
      //char mqttMsg[] = "faszo";
      char mqttMsgBuffer[200];
      snprintf(mqttMsgBuffer, sizeof(mqttPayload)/sizeof(char), "%s\r", mqttPayload);
      return mqttMsgBuffer;
      }*/

    int readSerial() {
      if (!GSMModemSerial.available()) {
        return -1;
      }
      //serInBuff[0] = '\0';
      unsigned int waitCharTime = 0;

      while (GSMModemSerial.available() || (waitCharTime < 100)) {
        if (!GSMModemSerial.available()) {
          waitCharTime++;
          if (waitCharTime >= 100) {
            return 0;
          }
          delay(1);     ///TODO: Blocking delay here!
          continue;
        }
        waitCharTime = 0;

        serInBuff[buffIndex] = (char)GSMModemSerial.read();
#ifdef _SIMDEBUG_PRINTRAW
        Serial.print((char) serInBuff[buffIndex]);
#endif
        buffIndex++;
        if (!(buffIndex < IN_BUFF_LEN)) {
          Serial.println("[Er] in buff full");
          return -1;
        }
      }
      return 0;

    }
    int debugPrintResponse() {
      while (GSMModemSerial.available()) {
        Serial.print((char)GSMModemSerial.read());
      }
    }

    void dbgPrintGSMBuffer() {
      Serial.println("");
      Serial.print("GSM| ");
      for (int i = 0; i < (IN_BUFF_LEN - 1); i++) {
        if (serInBuff[i] == '\0') {
          break;
        }
        Serial.print((char)serInBuff[i]);
        if (serInBuff[i] == '\n' && serInBuff[i + 1] != '\0') {
          Serial.print("GSM| ");
        }
      }
      //Serial.println(" ");
      return;
    }

    int atPrint(const char* atCommand, const char* responseOKKey, uint32_t timeout = 5000, const char* responseFAILKey = "ERROR") {
#define _SIMDEBUG 1
      delay(100);
      timeLimitms = timeout;
      GSMModemSerial.print(atCommand);
      startedMs = GETMILISECS();

#ifdef _SIMDEBUG
      Serial.print("\nSending: ");
      Serial.println(atCommand);
      debugPrintResponse();
#endif
      
      
      int8_t res = aTresp_unknown;
      
      this->buffIndex = 0;
      serInBuff[buffIndex] = '\0';
      
      while ((res != aTresp_ok) && (res != aTresp_failKeyFound) && (GETELAPSEDMS() < this->timeLimitms)) {
        if (readSerial() != 0) {
          delay(10);
          continue;
        }
        serInBuff[buffIndex] = '\0';

        /* first look for fail key, if fail key is present exit. */
        if (responseFAILKey != "___" && strstr(serInBuff, responseFAILKey) != NULL) {
          res = aTresp_failKeyFound;
          break;
        }
        else if (strstr(serInBuff, responseOKKey) != NULL) {
          res = aTresp_ok;
          break;
        }
      }

      if((res != aTresp_ok) && (res != aTresp_failKeyFound)){
   
        if(this->buffIndex == 0){
          res = aTresp_noResponse;
        }
        else if(readSerial() == 0){
          res = aTresp_timeout;
        }else{
          res = aTresp_noKeyFound;
        }
      }

      serInBuff[buffIndex] = '\0';  /// add terminator at the end of str
      dbgPrintGSMBuffer();

      //#ifdef _SIMDEBUG
      Serial.print("AT response: ");
      switch (res) {
        case aTresp_unknown:
          Serial.println(F("[ UNKNOWN ]"));
          break;
        case aTresp_noResponse:
          Serial.println(F("[ NO RESPONSE ]"));
          break;
        case aTresp_timeout:
          Serial.println(F("[ TIMEOUT ]"));
          break;
        case aTresp_noKeyFound:
          Serial.println(F("[ NO KEY ]"));
          break;
        case aTresp_failKeyFound:
          Serial.println(F("[ FAIL KEY ]"));
          break;
        case aTresp_ok:
          Serial.println(F("[ OK ]"));
          break;
        default:
          break;
      }
      Serial.print("elapsed: ");
      Serial.println(GETELAPSEDMS());
      //#endif

      return res;
    }

    /* same as ATprint but perform wakeup first if needed*/
    int atSend(const char* atCommand, const char* responseOKKey, uint32_t timeout = 5000, const char* responseFAILKey = "ERROR") {

      int res;

      res = this->atPrint(atCommand,responseOKKey,timeout,responseFAILKey);
      if(res != aTresp_noResponse){
        return res;
      }
      
      int wakeCount = 0;
      for (; wakeCount < 10; wakeCount++) {
        if (this->atPrint("AT\r", "OK", 500) == 0) {
          delay(200);
          res = this->atPrint(atCommand,responseOKKey,timeout,responseFAILKey);
          if(res != aTresp_noResponse){
            wakeCount = 10;
            break;
          }
        }
        else{
          delay(random(100, 5000));
        }
      }

      if(!(wakeCount < 10)){
        Serial.println("[ ER ] modem is not responding to AT");
      }

      if(res == aTresp_noResponse){
        atNoResponseCounter++;
      }

      return res;
    }


    int32_t datetimeToUnixTime(char* datetime_str) {  /// CHAT GPT did this method, if the code sux it's not my fault.
      struct tm tm;
      time_t t;

      int val;

      // Year
      if (sscanf(datetime_str, "%4d", &val) != 1) {
        fprintf(stderr, "Failed to parse year in datetime string: %s\n", datetime_str);
        return -1;
      }
      tm.tm_year = val - 1900;
      datetime_str += 4;

      // Month
      if (sscanf(datetime_str, "%2d", &val) != 1) {
        fprintf(stderr, "Failed to parse month in datetime string: %s\n", datetime_str);
        return -1;
      }
      tm.tm_mon = val - 1;
      datetime_str += 2;

      // Day
      if (sscanf(datetime_str, "%2d", &val) != 1) {
        fprintf(stderr, "Failed to parse day in datetime string: %s\n", datetime_str);
        return -1;
      }
      tm.tm_mday = val;
      datetime_str += 2;

      // Hour
      if (sscanf(datetime_str, "%2d", &val) != 1) {
        fprintf(stderr, "Failed to parse hour in datetime string: %s\n", datetime_str);
        return -1;
      }
      tm.tm_hour = val;
      datetime_str += 2;

      // Minute
      if (sscanf(datetime_str, "%2d", &val) != 1) {
        fprintf(stderr, "Failed to parse minute in datetime string: %s\n", datetime_str);
        return -1;
      }
      tm.tm_min = val;
      datetime_str += 2;

      // Second
      if (sscanf(datetime_str, "%2d", &val) != 1) {
        fprintf(stderr, "Failed to parse second in datetime string: %s\n", datetime_str);
        return -1;
      }
      tm.tm_sec = val;

      // Convert struct tm to time_t
      t = mktime(&tm);
      if (t == -1) {
        fprintf(stderr, "Failed to convert struct tm to time_t\n");
        return -1;
      }

      // Convert the timestamp to int32_t
      if (t < INT32_MIN || t > INT32_MAX) {
        fprintf(stderr, "Timestamp out of range for int32_t\n");
        return -1;
      }
      return (int32_t)t;
    }

    int32_t getUnixTmNTP(int32_t &timeBuff) {
      /// @TODO: to be implemented!
    }
    int getUnixTm(int32_t &timeBuff) {

      //Serial.println("MODEMTIME");

      char returnStr[20] = "";
     /* this->getDataStr(returnStr, 0, true);
      this->getDataStr(returnStr, 1);
      this->getDataStr(returnStr, 2);

      this->getDataStr(returnStr, 4);
      this->getDataStr(returnStr, 5);
      this->getDataStr(returnStr, 6);
      this->getDataStr(returnStr, 7);
      this->getDataStr(returnStr, 8);
      this->getDataStr(returnStr, 9);
      this->getDataStr(returnStr, 10);*/

      this->getDataStr(returnStr, 3, true);
      //Serial.println(strlen(returnStr));
      if (this->getDataStr(returnStr, 3) == 0 && strlen(returnStr) == 18) { /// UTC is probably valid, worth trying parsing
        int32_t newUnixTime = datetimeToUnixTime(returnStr);
        if (newUnixTime != -1) {
          timeBuff = newUnixTime;
          return 0;                     /// time is parsed correctly
        }
      }
      Serial.println(F("Error getting time from GNSS"));
      return -1;
    }
};
