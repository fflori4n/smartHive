class BasicHiveSensor {
#define CHKDELAY 500

  private:
    char sensorAddressChar;
    char rawResponseBuff[100];

    bool isUpdated = false;
    int sensorNoConnectionCounter = 0;

    char* sensName = "UNNAMED";

    RS485Com &serial;
    uint8_t readTimeOut;
    uint8_t numRetryIfNoResponse = 1;

    double dhtCsvData[8];
    const char labels[9][3] = {"T0", "H0", "T1", "H1", "T2", "H2", "TS", "MO", "NC"};

#define StoD_BUFF_LEN 6
    double string2Double(char (&strBuff)[StoD_BUFF_LEN]) {
      byte multipl = 1;
      double numb = 0;

      for (int i = 0; i < StoD_BUFF_LEN; i++) {
        if (strBuff[StoD_BUFF_LEN - 1 - i] == '-') {
          numb *= -1;
          continue;
        }
        double digit = (double)((strBuff[StoD_BUFF_LEN - 1 - i] - '0'));
        if (digit < 0 || digit > 9) {
          continue;
        }
        numb += digit * multipl;
        multipl *= 10;
        //Serial.println(digit);
      }
      return numb;
    }
    void parseCSV() {
      //uint8_t len = serial.getMSGLen();
      uint8_t cCount = 0;
      uint8_t strIndex = 0;
      char strToDoubleBuff[StoD_BUFF_LEN] = "";
      char c = serial.get(0);                                       /// !! very cursed - TODO: maybe?


      for (int i = 0 ; c != '\0'; c = serial.get(++i)) {
        if (c == ',' || c == '\0') { //
          cCount++;
          //Serial.println(cCount);
          strIndex = 0;
          switch (cCount) {
            case 2:
              dhtCsvData[0] = string2Double(strToDoubleBuff) / 10.0;
              break;
            case 3:
              dhtCsvData[1] = string2Double(strToDoubleBuff) / 10.0;
              break;
            case 4:
              dhtCsvData[2] = string2Double(strToDoubleBuff) / 10.0;
              break;
            case 5:
              dhtCsvData[3] = string2Double(strToDoubleBuff) / 10.0;
              break;
            case 6:
              dhtCsvData[4] = string2Double(strToDoubleBuff) / 10.0;
              break;
            case 7:
              dhtCsvData[5] = string2Double(strToDoubleBuff) / 10.0;
              break;
            case 8:
              dhtCsvData[6] = string2Double(strToDoubleBuff);
              break;
            case 9:
              dhtCsvData[7] = string2Double(strToDoubleBuff);
              break;
          }
          for (int i = 0; i < StoD_BUFF_LEN; i++) {
            strToDoubleBuff[i] = '\0';
          }
          //Serial.println("buff cleared!");
          continue;
        }
        strToDoubleBuff[strIndex] = c;
        strIndex++;
        /*Serial.print("buffPlaced:");
          Serial.println(c);*/
      }

      //Serial.println(strToDoubleBuff);
      dhtCsvData[7] = string2Double(strToDoubleBuff);   /// TODO: !! This is a dirty fix, find out why doesn't the loop work for last tag


      for (int i = 0; i < 8; i++) {
        Serial.println(dhtCsvData[i]);
      }
    }
  public:

    BasicHiveSensor(const char &address, char* sensName, RS485Com &serialUsed, uint8_t readTimeOut = 5000, uint8_t numRetryIfNoResponse = 5) : serial(serialUsed) {
      if (address == '\0') {
        Serial.println(F("Sensor can't have address 0"));
        return;
      }
      sensorAddressChar = address;
      this->sensName = sensName;
      this->readTimeOut = readTimeOut;
      this->numRetryIfNoResponse = numRetryIfNoResponse;
    }

    int update() {

      for (int pollTry = 0; pollTry < numRetryIfNoResponse; pollTry++) {
        if (serial.pollSensor(sensorAddressChar, (char)70) == 0) {
          /// wait for x millis for sensor response.
          for (int i = 0; i < readTimeOut; i += CHKDELAY) {
            if (serial.checkInbuf('A', sensorAddressChar) == 0) { /// only accept response from TYPE A - Basic hive sensor and from this->sensor's address
              Serial.println(F("[ OK ] Sensor reachable"));
              parseCSV();
              isUpdated = true;
              return 0;
            }
            delay(CHKDELAY);
          }
        }
        else {
          Serial.println(F("[ ER ] poll req. timeout: "));
          Serial.print(sensorAddressChar);
        }
      }
      isUpdated = false;
      return -1;

    }

    void doubleToStr(char(& outBuff)[15], double &dNumber) {
      dtostrf(dNumber, 4, 2, outBuff);
    }

    void addMqttTags(char(& mqttPayloadBuff)[500], char(& tempStrBuffer)[_TEMP_STRLEN]) {
      char doubleConvBuff[15];

      if(!isUpdated){   /// do not add values if sensor was unreachable at last poll.
        sensorNoConnectionCounter++;
        snprintf(tempStrBuffer, _TEMP_STRLEN, " \"%s_%s\":%d,", sensName, labels[8], sensorNoConnectionCounter);
        strlcat(mqttPayloadBuff, tempStrBuffer, 500);
        return;
      }
      sensorNoConnectionCounter = 0;

      for (int i = 0; i < 8; i++) {               /// INCOMING SENSOR DATA
        if (dhtCsvData[i] == -99.9) {             /// do not send sensor data, if sensor is inactive
          continue;
        }
        doubleToStr(doubleConvBuff, dhtCsvData[i]);
        snprintf(tempStrBuffer, _TEMP_STRLEN, " \"%s_%s\":%s,", sensName, labels[i], doubleConvBuff);
        strlcat(mqttPayloadBuff, tempStrBuffer, 500);
      }

      /// Additional info from HUB related to sensor connectivity.
      snprintf(tempStrBuffer, _TEMP_STRLEN, " \"%s_%s\":%d,", sensName, labels[8], sensorNoConnectionCounter);
      strlcat(mqttPayloadBuff, tempStrBuffer, 500);
    }
};
