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

class SIM7000 {
  private:
    char serInBuff[IN_BUFF_LEN];

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


  public:

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

      uint8_t res = -2;
      this->buffIndex = 0;
      serInBuff[0] = '\0';
      while ( res != 0 && (GETELAPSEDMS() < this->timeLimitms)) {
        if (readSerial() != 0) {
          delay(10);
          continue;
        }
        serInBuff[buffIndex] = '\0';
        atCmdRes = -2;

        if (responseFAILKey != "___" && strstr(serInBuff, responseFAILKey) != NULL) {
          atCmdRes = -1;
          break;
        }
        if (strstr(serInBuff, responseOKKey) != NULL) {
          atCmdRes = 0;
          break;
        }
      }
      serInBuff[buffIndex] = '\0';  /// add terminator at the end of str
      dbgPrintGSMBuffer();

      //#ifdef _SIMDEBUG
      Serial.print("AT response: ");
      switch (atCmdRes) {
        case 0:
          Serial.print("[ OK ]");
          break;
        case -1:
          Serial.print("[ FAIL ]");
          break;
        case -2:
          Serial.print("[ UNKNOWN ]");
          break;
        case -3:
          Serial.print("[ NO RESP. ]");
          break;
      }
      Serial.print("\nelapsed: ");
      Serial.println(GETELAPSEDMS());
      //#endif

      return atCmdRes;
    }

    int32_t datetime_to_unix_timestamp(char* datetime_str) {
    struct tm tm;
    time_t t;

    // Parse the datetime string
    char* p = datetime_str;
    int val;

    // Year
    if (sscanf(p, "%4d", &val) != 1) {
        fprintf(stderr, "Failed to parse year in datetime string: %s\n", datetime_str);
        return -1;
    }
    tm.tm_year = val - 1900;
    p += 4;

    // Month
    if (sscanf(p, "%2d", &val) != 1) {
        fprintf(stderr, "Failed to parse month in datetime string: %s\n", datetime_str);
        return -1;
    }
    tm.tm_mon = val - 1;
    p += 2;

    // Day
    if (sscanf(p, "%2d", &val) != 1) {
        fprintf(stderr, "Failed to parse day in datetime string: %s\n", datetime_str);
        return -1;
    }
    tm.tm_mday = val;
    p += 2;

    // Hour
    if (sscanf(p, "%2d", &val) != 1) {
        fprintf(stderr, "Failed to parse hour in datetime string: %s\n", datetime_str);
        return -1;
    }
    tm.tm_hour = val;
    p += 2;

    // Minute
    if (sscanf(p, "%2d", &val) != 1) {
        fprintf(stderr, "Failed to parse minute in datetime string: %s\n", datetime_str);
        return -1;
    }
    tm.tm_min = val;
    p += 2;

    // Second
    if (sscanf(p, "%2d", &val) != 1) {
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
   /* int32_t datetime_to_unix_timestamp() {  /// written by chatGPT
      struct tm tm;
      time_t t;

      char datetime_str[] = "20230325234021.000"; 
      // Parse the datetime string
      if (strptime("20230325234021", "%Y%m%d%H%M%S", &tm) == NULL) {
        fprintf(stderr, "Failed to parse datetime string: %s\n", datetime_str);
        return -1;
      }

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
    }*/

    int32_t getUnixTmNTP(int32_t &timeBuff) {
    }
    int getUnixTm(int32_t &timeBuff) {

      Serial.println("MODEMTIME");

      char datetimestr[]= "20230325234021.000";
      Serial.println(datetime_to_unix_timestamp("20230325234021"));
      if (this->wakeUpAT() != 0) {
        Serial.print("Can't wake up");
        return -1;
      }
      //this->atPrint("AT+CGNSINF=?\r", "xxx");


      /// GET GNSS INFO
      this->atPrint("AT+CGNSPWR=1\r", "OK");
      int res = this->atPrint("AT+CGNSINF\r", "+CGNSINF: 1,1", 5000, "+CGNSINF: 1,0");


      if (res == -1) {
        Serial.println("GNSS| PWR on, No fix.");
      }
      else if (res == -2 || res == -3) {
        Serial.println("GNSS| No fix, unknown.");
      }
      Serial.println(this->getInBuffer());


      return 0;
    }
};
