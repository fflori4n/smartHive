#include <HardwareSerial.h>

typedef struct tSim7000gInformation {

  uint16_t gnss_utcTime;
  double gnss_latitude;
  double gnss_longitude;
  double gnss_masl;
  double gnss_groundSpeed;
  double gnss_course;
  int gnss_satelites;
  int gnss_cnoMax;

  int sim_rssi;

  struct tm gnss_timeinfo;
  time_t gnss_unixTime;

} tSim7000gInformation;

class SimModem {

#define GETMILISECS() (esp_timer_get_time()/1000)  /// efficient? Yes. Cursed? Also yes.
#define GETELAPSEDMS(startedAt) ((esp_timer_get_time()/1000) - startedAt)

#define GSMBAUD 9600
#define GSMRX_PIN 16
#define GSMTX_PIN 17

#define SIM_MODEM_SOFTTX_BUFF_SIZE 2048
#define SIM_MODEM_SOFTRX_BUFF_SIZE 2048
#define SIM_MAX_AT_WAKEUP_RETRY 5
#define SIM_MODEM_WAIT_DELAY 50
#define SIM_MAX_PAUSE_BETWEEN_CHARS_INRESPONSE_MS 200  /*slow?*/
#define SIM_TERMINATE_OK_R true
#define MQTT_PAYLOAD_BUFF_LEN 2048
  public:
    tSim7000gInformation simformation;

  private:

    HardwareSerial simModem = NULL;

    char comOutBuffer[SIM_MODEM_SOFTTX_BUFF_SIZE] = {'\0'};
    char comInBuffer[SIM_MODEM_SOFTRX_BUFF_SIZE] = {'\0'};
    char responseCVSmember[32] = {'\0'};

    uint32_t stopWatchStart = 0;

    enum tResult {
      res_positive,
      res_negative,
      res_timeout,
      res_error,
      res_unknown
    };

    tResult clearRxBuffer() {
      while (this->simModem.available()) {
        delayMicroseconds(100);
        this->simModem.read();
      }

      return res_positive;
    }

    tResult waitForResponse(const char* positiveRespKey, const char* negativeRespKey, const char* stopKey, uint16_t timeout = 5000, boolean muteDbgSerial = false) {

      tResult result = res_timeout;

      this->stopWatchStart = GETMILISECS();

      memset(this->comInBuffer, '\0', sizeof(this->comInBuffer));

      uint16_t responseTime = 0;
      uint16_t comInIndex = 0;
      uint16_t waitForResponseEnd = 0;

      for (; ((res_timeout == result) && (responseTime < (timeout / SIM_MODEM_WAIT_DELAY))); responseTime++) {

        if (this->simModem.available()) {

          char charRecieved = (char)this->simModem.read();

          delayMicroseconds(100);
          comInBuffer[comInIndex] = charRecieved;

          if ((comInIndex + 1) >= SIM_MODEM_SOFTRX_BUFF_SIZE) {
            result = res_error;
            Serial.print(F("SIM_RESP|RX BUFF FULL"));
          }
          else {
            comInIndex++;
          }

          waitForResponseEnd = 0;
        }
        else {

          /*if (0 != comInIndex) {
            if (waitForResponseEnd < SIM_MAX_PAUSE_BETWEEN_CHARS_INRESPONSE_MS) {
              waitForResponseEnd += SIM_MODEM_WAIT_DELAY;
            }
            else {
              result = res_unknown;
            }
            }*/
          if ((stopKey != NULL) && (strstr(comInBuffer, stopKey) != NULL)) {
            if ((negativeRespKey != NULL) && (strstr(comInBuffer, negativeRespKey) != NULL)) {
              result = res_negative;
            }
            else if ((positiveRespKey != NULL) && (strstr(comInBuffer, positiveRespKey) != NULL)) {
              result = res_positive;
            }
          }
          vTaskDelay(SIM_MODEM_WAIT_DELAY / portTICK_PERIOD_MS);

        }
      }

      if((stopKey != NULL) && (strstr(comInBuffer, stopKey) == NULL)){
        Serial.println(F("SIM_RESP| NO STOP.KEY - WAIT TMOUT [WR]"));
      }
      if ((negativeRespKey != NULL) && (strstr(comInBuffer, negativeRespKey) != NULL)) {
        result = res_negative;
      }
      else if ((positiveRespKey != NULL) && (strstr(comInBuffer, positiveRespKey) != NULL)) {
        result = res_positive;
      }

      responseTime = GETELAPSEDMS(this->stopWatchStart);

      if (false == muteDbgSerial) {
        switch (result) {
          case res_positive:
            Serial.print(F("SIM_RESP| OK    |ms:"));
            Serial.print(responseTime);
            Serial.println(F("|recieved:"));
            break;
          case res_negative:
            Serial.print(F("SIM_RESP| NO OK |ms:"));
            Serial.print(responseTime);
            Serial.println(F("|recieved:"));
            break;
          case res_timeout:
            Serial.print(F("SIM_RESP| TIMEOUT |ms:"));
            Serial.print(responseTime);
            Serial.println(F("|recieved:"));
            break;
          case res_error:
            Serial.print(F("SIM_RESP|  ERROR  |ms:"));
            Serial.print(responseTime);
            Serial.println(F("|recieved:"));
            break;
          default:
            break;
        }
        if (comInBuffer[0] != '\0') {
          Serial.println(F("[["));
          Serial.println(comInBuffer);
          Serial.println(F("]]"));
        }
      }


      return result;
    }

    tResult evaluateRxBufferContent(const char* positiveRespKey, const char* negativeRespKey) {

      tResult result = res_unknown;

      if ((positiveRespKey != NULL) && (strstr(comInBuffer, positiveRespKey) != NULL)) {
        result = res_positive;
      }
      else if ((negativeRespKey != NULL) && (strstr(comInBuffer, negativeRespKey) != NULL)) {
        result = res_negative;
      }

      Serial.print(F("SIM_RX_EVAL|"));
      Serial.println(result);
      return result;
    }

    tResult sendATwakeup() {

      tResult result = res_timeout;

      Serial.print("SIM|Sending wakeup");

      this->simModem.flush(); /* fush TX buffer, probably not important but better safe.*/
      vTaskDelay(3000 / portTICK_PERIOD_MS);

      for (uint16_t i = 0; ((i < SIM_MAX_AT_WAKEUP_RETRY) && (res_timeout == result)); i++) {

        const char* payload = "AT\r";

        Serial.print(".");

        if (!(this->simModem)) {
          result = res_error;
          continue;
        }

        if (NULL == strstr(payload, "\r")) {
          Serial.print("SIM|NO \r in payload [WR]");
        }

        size_t payloadSize = strnlen(payload, 65535u);
        clearRxBuffer();
        const size_t written = this->simModem.write(payload, (size_t)payloadSize);
        this->simModem.flush(); /* fush TX buffer, probably not important but better safe.*/

        result = waitForResponse("OK", NULL, "OK", 500);

        if (res_timeout == result) {
          vTaskDelay(random(50, 3000) / portTICK_PERIOD_MS);
        }
      }

      return result;
    }

    tResult getSubstring(const char *input, const char *startKey, const char *endKey, char *output) {
      const char *startPos = strstr(input, startKey);
      if (startPos == NULL) {
        return res_error;
      }

      startPos += strlen(startKey);

      const char *endPos = strstr(startPos, endKey);
      if (endPos == NULL) {
        return res_error;
      }
      if ((endPos - startPos) > 32) {
        return res_error;
      }
      strncpy(output, startPos, (endPos - startPos));
      output[endPos - startPos] = '\0'; // Null-terminate the output string
      return res_positive;
    }

    const char* pointerToNth(const char* str, char x, uint8_t n) {

      while (*str) {
        if (*str == x) {
          if (n == 0) {
            return str;
          }
          n--;
        }
        str++;
      }
      return str;

    }


  public:

    SimModem(HardwareSerial & modemSerial) {
      this->simModem = modemSerial;
    }

    int8_t begin(const uint16_t baud) {

      if (!(this->simModem)) {
        this->simModem.begin(baud, SERIAL_8N1, GSMRX_PIN, GSMTX_PIN);
      }
      else {
        Serial.println("SIM|Serial exists [WR]");
        return (int8_t)res_positive;
      }


      for (uint8_t i = 0; i < (500 / SIM_MODEM_WAIT_DELAY); i++) {

        if (this->simModem) {

          /*this->simModem.clear(); /* clear TX and RX buffers */
          Serial.println("SIM|SERIAL STARTED [OK]");
          return (int8_t)res_positive;
        }
        vTaskDelay(SIM_MODEM_WAIT_DELAY / portTICK_PERIOD_MS);
      }
      Serial.println("SIM|Serial init timeout [WR]");
      return (int8_t)res_error;
    }

    int8_t atPrintNoCheck(const char* payload) {

      Serial.print("SIM|Sending: ");
      Serial.println(payload);

      if (!(this->simModem)) {
        return (int8_t)res_error;
      }

      if (NULL == strstr(payload, "\r")) {
        Serial.print("SIM|NO \r in payload [WR]");
      }

      size_t payloadSize = strnlen(payload, 65535u);
      clearRxBuffer();
      const size_t written = this->simModem.write(payload, (size_t)payloadSize);

      /*     while (this->simModem.available()) {
             Serial.print((char)this->simModem.read());
           }*/

      this->simModem.flush(); /* fush TX buffer, probably not important but better safe.*/
      return (int8_t)res_positive;
    }

    /* Blocking while waiting for response! return result */
    int8_t atPrint(const char* payload, const char* positiveResp, const char* negativeResp = "ERROR", const char* txStopped = NULL, uint32_t timeout = 5000 ) {

      Serial.print("SIM|Sending: ");
      Serial.println(payload);

      if (!(this->simModem)) {
        return (int8_t)res_error;
      }

      if (NULL == strstr(payload, "\r")) {
        Serial.print("SIM|NO \r in payload [WR]");
      }

      size_t payloadSize = strnlen(payload, 65535u);
      clearRxBuffer();
      const size_t written = this->simModem.write(payload, (size_t)payloadSize);

      this->simModem.flush(); /* fush TX buffer, probably not important but better safe.*/
      return (int8_t)waitForResponse(positiveResp, negativeResp, txStopped, timeout);
    }

    int8_t atPrintWRetry(const char* payload, const char* positiveResp, const char* negativeResp = "ERROR", const char* txStopped = NULL, uint32_t timeout = 5000, uint8_t retryNum = 5 ) {

      tResult result = res_timeout;

      for (uint8_t i = 0; ((i < retryNum) && (res_positive != result)); i++) {

        Serial.print("SIM|Sending: ");
        Serial.println(payload);

        if (NULL == strstr(payload, "\r")) {
          Serial.print("SIM|NO \r in payload [WR]");
        }

        if (!(this->simModem)) {
          return (int8_t)res_error;
        }

        size_t payloadSize = strnlen(payload, 65535u);
        clearRxBuffer();
        const size_t written = this->simModem.write(payload, (size_t)payloadSize);

        this->simModem.flush(); /* fush TX buffer, probably not important but better safe.*/
        result = waitForResponse(positiveResp, negativeResp, txStopped, timeout);

        if (res_timeout == result) {
          sendATwakeup();
        }
      }

      return result;
    }

    int8_t getRSSI() {

      const int RSSILUT[] = {115, 111, 110, 109, 107, 105, 103, 101, 99, 97, 95, 93, 91, 89, 87, 85, 83, 81, 79, 77, 75, 73, 71, 69, 67, 65, 63, 61, 59, 57, 55, 53};

      tResult res = (tResult)(this->atPrintWRetry("AT+CSQ\r", "+CSQ:", "ERROR","OK\r", 20000, 1));

      if (res_positive != res) {
        return -1;
      }

      char* response = strstr(this->comInBuffer, "+CSQ:") + strlen("+CSQ:");
      if (NULL == response) {
        return -1;
      }
      int rssiRaw = 0;
      getSubstring(response, " ", ",", responseCVSmember);  /* rssi */
      sscanf(responseCVSmember, "%d", &rssiRaw);
      Serial.println(rssiRaw);

      (rssiRaw < 32) ? (simformation.sim_rssi = RSSILUT[rssiRaw]) : (simformation.sim_rssi = 0);  /// convert SIM response to dBm or -99
      Serial.println(simformation.sim_rssi);
      return 0;
    }

    int8_t getUnixTime() {
    }
    int8_t getGNSSData() {

      tResult res = (tResult)(this->atPrintWRetry("AT+CGNSINF\r", "+CGNSINF: 1", "+CGNSINF: 0","OK\r", 20000, 1));

      switch (res) {
        case res_negative:
          this->atPrintWRetry("AT+CGNSPWR=1\r", "OK", "ERROR","OK\r", 5000, 4);
          return -1;
          break;
        case res_positive:
          break;
        default:
          return -1;
          break;
      }

      res = evaluateRxBufferContent("+CGNSINF: 1,1", "+CGNSINF: 1,0");

      /*char responseCVSmember[32] = {'\0'};*/
      char* response = strstr(this->comInBuffer, "+CGNSINF: 1,1") + strlen("+CGNSINF: 1,1");
      if (NULL == response) {
        return -1;
      }

      Serial.print("GNSS str:");
      Serial.print(response);

      getSubstring(pointerToNth(response, ',', 0), ",", ".", responseCVSmember);

      sscanf(responseCVSmember, "%4d%2d%2d%2d%2d%2d", &simformation.gnss_timeinfo.tm_year, &simformation.gnss_timeinfo.tm_mon, &simformation.gnss_timeinfo.tm_mday, &simformation.gnss_timeinfo.tm_hour, &simformation.gnss_timeinfo.tm_min, &simformation.gnss_timeinfo.tm_sec);

      simformation.gnss_timeinfo.tm_isdst = -1;
      simformation.gnss_timeinfo.tm_year -= 1900;
      simformation.gnss_timeinfo.tm_mon -= 1;

      // Convert tm struct to Unix timestamp
      simformation.gnss_unixTime = mktime(&simformation.gnss_timeinfo);
      Serial.println("gnss unix timestamp:");
      Serial.println(simformation.gnss_unixTime);

      /*Serial.println(simformation.gnss_timeinfo.tm_year);
        Serial.println(simformation.gnss_timeinfo.tm_mon);
        Serial.println(simformation.gnss_timeinfo.tm_mday);
        Serial.println(simformation.gnss_timeinfo.tm_hour);
        Serial.println(simformation.gnss_timeinfo.tm_min);
        Serial.println(simformation.gnss_timeinfo.tm_sec);*/

      getSubstring(pointerToNth(response, ',', 1), ",", ",", responseCVSmember);  /* latitude */
      sscanf(responseCVSmember, "%lf", &simformation.gnss_latitude);
      Serial.println(simformation.gnss_latitude);/**1000000*/

      getSubstring(pointerToNth(response, ',', 2), ",", ",", responseCVSmember);  /* longitude */
      sscanf(responseCVSmember, "%lf", &simformation.gnss_longitude);
      Serial.println(simformation.gnss_longitude);

      getSubstring(pointerToNth(response, ',', 3), ",", ",", responseCVSmember);  /* MASL */
      sscanf(responseCVSmember, "%lf", &simformation.gnss_masl);
      Serial.println(simformation.gnss_masl);

      getSubstring(pointerToNth(response, ',', 4), ",", ",", responseCVSmember);  /* Speed */
      sscanf(responseCVSmember, "%lf", &simformation.gnss_groundSpeed);
      Serial.println(simformation.gnss_groundSpeed);

      getSubstring(pointerToNth(response, ',', 5), ",", ",", responseCVSmember);  /* Course */
      sscanf(responseCVSmember, "%lf", &simformation.gnss_course);
      Serial.println(simformation.gnss_course);

      simformation.gnss_satelites = 0;
      getSubstring(pointerToNth(response, ',', 12), ",", ",", responseCVSmember);  /* Course */
      sscanf(responseCVSmember, "%d", &simformation.gnss_satelites);
      Serial.println(simformation.gnss_satelites);

      getSubstring(pointerToNth(response, ',', 16), ",", ",", responseCVSmember);  /* Course */
      sscanf(responseCVSmember, "%d", &simformation.gnss_cnoMax);
      Serial.println(simformation.gnss_cnoMax);

      return (int8_t)res;
    }

    int8_t setBaseConfig() {

      tResult res = (tResult)(this->atPrintWRetry("AT+IPR=9600\r", "OK", "ERROR","OK\r", 5000, 4));
      res = (tResult)(this->atPrintWRetry("ATE1\r", "OK", "ERROR","OK\r", 5000, 4));

      if (res_positive == (tResult)(this->atPrintWRetry("AT+CREG?\r", "+CREG: 0,1", "ERROR", "OK\r",30000, 1))) {
        return 0;
      }
      /*res = (tResult)(this->atPrintWRetry("AT+CEDUMP=0\r", "OK", "ERROR", 5000, 4));*/
      res = (tResult)(this->atPrintWRetry("AT+CFUN=1,1\r", "OK", "ERROR","OK\r", 15000, 1));

      return 0;
    }

    int8_t connectToNetwork() {

      tResult res = (tResult)(this->atPrintWRetry("AT+CNACT?\r", "+CNACT: 1", "0.0.0.0","OK\r", 5000, 4));

      if (res_positive == res) {
        return 0;
      }
      else if (res_negative == res) {

        /* authenticate sim and connect to carrier */

        res = (tResult)(this->atPrintWRetry("AT+CPIN?\r", "OK", "ERROR", "OK\r",5000, 4));
        res = (tResult)(this->atPrintWRetry("AT+CNMP=13\r", "OK", "ERROR", "OK\r",5000, 4));
        res = (tResult)(this->atPrintWRetry("AT+CNACTCFG=IPV4V6\r", "OK", "ERROR", "OK\r",10000, 4));
        res = (tResult)(this->atPrintWRetry("AT+CREG?\r", "+CREG: 0,1", "ERROR", "OK\r",30000, 4));
        res = (tResult)(this->atPrintWRetry("AT+CSQ\r", "OK", "ERROR", "OK\r",5000, 4));

        /* set APN config and connect to the internet */
        res = (tResult)(this->atPrintWRetry("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r", "OK", "ERROR","OK\r", 5000, 4));
        res = (tResult)(this->atPrintWRetry("AT+SAPBR=3,1,\"APN\",\"mts\"\r", "OK", "ERROR", "OK\r",5000, 4));
        res = (tResult)(this->atPrintWRetry("AT+SAPBR=3,1,\"PWD\",\"064\"\r", "OK", "ERROR","OK\r", 5000, 4));
        res = (tResult)(this->atPrintWRetry("AT+CGATT?\r", "+CGATT: 1", NULL, "OK\r",20000, 4));
        res = (tResult)(this->atPrintWRetry("AT+CNACT=1,\"mts\"\r", "+CGATT: 1", "ERROR","+APP PDP:", 30000, 4));

        return (int8_t)res;

      }
      else {
        /* timeout or error or unknown */
        return -1;
      }
    }

    int8_t sendMqtt(char(& mqttPayloadBuff)[MQTT_PAYLOAD_BUFF_LEN], const char* mqttTopic) {
      /* TODO: values are hardcoded for simplicity. */
      /*check if config is already set, this is done by checking if the URL is set chances are other params are also set. */
      tResult res = (tResult)(this->atPrintWRetry("AT+SMCONF?\r", "fflori4nhahomelab.duckdns.org", "0.0.0.0","OK\r", 15000, 2));
      vTaskDelay(100 / portTICK_PERIOD_MS);
      if ((res_negative == res) || (res_timeout == res)) {
        res = (tResult)(this->atPrint("AT+SMCONF=\"URL\",fflori4nhahomelab.duckdns.org,1883\r", "OK", NULL, "OK\r", 15000));
        res = (tResult)(this->atPrint("AT+SMCONF=\"CLIENTID\",\"ESP_DEV0\"\r", "OK", NULL,"OK\r", 15000));
        res = (tResult)(this->atPrint("AT+SMCONF=\"KEEPTIME\",60\r", "OK", NULL,"OK\r", 15000));
      }

      /* connect to broker */
      res = (tResult)(this->atPrintWRetry("AT+SMCONN\r", "OK", "ERROR", "OK\r", 35000, 1));
      vTaskDelay(5000 / portTICK_PERIOD_MS);


      snprintf(this->comOutBuffer, SIM_MODEM_SOFTTX_BUFF_SIZE, "AT+SMPUB=\"%s\",\"%d\",1,1\r", mqttTopic, strlen(mqttPayloadBuff));
      Serial.print("MQTT| topic:");
      Serial.println(this->comOutBuffer);
      Serial.print("MQTT| sending:");
      Serial.println(mqttPayloadBuff);
      res = (tResult)(this->atPrint(this->comOutBuffer, ">", NULL,">", 5000));
      if (res_positive == res) {

        res = (tResult)(this->atPrint(mqttPayloadBuff, "OK", "ERROR","OK\r", 15000));
        vTaskDelay(5000 / portTICK_PERIOD_MS);
      }

      res = (tResult)(this->atPrintWRetry("AT+SMDISC\r", "OK", "ERROR", "OK\r", 5000, 1));

      return 0;
    }
};
