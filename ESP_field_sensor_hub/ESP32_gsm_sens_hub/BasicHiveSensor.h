class BasicHiveSensor{
  #define CHKDELAY 500
  
  private:
  char sensorAddressChar;
  char rawResponseBuff[100];
  
  RS485Com &serial;
  uint8_t readTimeOut;

  double dhtCsvData[8];
  //const char* labels = {"T0", "H0", "T1", "H1", "T2"}

  #define StoD_BUFF_LEN 6
  double string2Double(char (&strBuff)[StoD_BUFF_LEN]){
    byte multipl = 1;
    double numb = 0;
    
    for(int i = 0; i< StoD_BUFF_LEN; i++){
      if(strBuff[StoD_BUFF_LEN -1 -i] == '-'){
        numb*=-1;
        continue;
      }
      int digit = (int)(strBuff[StoD_BUFF_LEN -1 -i] - '0');
      /*Serial.print(digit);
      Serial.print(" ");*/
      /*Serial.print((char)(digit + '0'));
      Serial.print(" ");*/
      if(digit < 0 || digit > 9){
        continue;
      }
      numb+=digit*multipl;
      multipl*=10;
      //Serial.println(digit);
    }
    return numb;
  }
  void parseCSV(){
    //uint8_t len = serial.getMSGLen();
    uint8_t cCount = 0;
    uint8_t strIndex = 0;
    char strToDoubleBuff[StoD_BUFF_LEN] = "";
    char c = serial.get(0);                                       /// !! very cursed - TODO: maybe?
    for(int i=0 ; c != '\0'; c = serial.get(++i)){   
      if(c == ',' || c == '\0'){ // 
        switch(cCount){
          case 1:
            dhtCsvData[0] = string2Double(strToDoubleBuff)/10;
            break;
          case 2:
            dhtCsvData[1] = string2Double(strToDoubleBuff)/10;
            break;
          case 3:
            dhtCsvData[2] = string2Double(strToDoubleBuff)/10;
            break;
          case 4:
            dhtCsvData[3] = string2Double(strToDoubleBuff)/10;
            break;
          case 5:
            dhtCsvData[4] = string2Double(strToDoubleBuff)/10;
            break;
          case 6:
            dhtCsvData[5] = string2Double(strToDoubleBuff)/10;
            break;
           /*case 7:
            Serial.println(strToDoubleBuff);
            dhtCsvData[6] = string2Double(strToDoubleBuff);
            break;
           case 8:
            Serial.println(strToDoubleBuff);
            dhtCsvData[7] = string2Double(strToDoubleBuff);
            break;*/

          for(int i=0; i < StoD_BUFF_LEN; i++){
            strToDoubleBuff[i] = '\0';
          }
        }
        cCount++;
        strIndex = 0;
        continue;
      }
      strToDoubleBuff[strIndex] = c;
      strIndex++;
      //Serial.print(c);
    }
    for(int i=0; i<8; i++){
         Serial.println(dhtCsvData[i]);
    }
  }
  public:
  
  BasicHiveSensor(const char &address, RS485Com &serialUsed, uint8_t readTimeOut = 5000) : serial(serialUsed){
    if(address == '\0'){
      Serial.println(F("Sensor can't have address 0"));
      return;
    }
    sensorAddressChar = address;
    this->readTimeOut = readTimeOut;
  }

  int update(){
    if(serial.pollSensor(sensorAddressChar,(char)70) == 0){
      /// wait for x millis for sensor response.
      for(int i=0; i < readTimeOut; i+= CHKDELAY){
        if(serial.checkInbuf('A',sensorAddressChar) == 0){    /// only accept response from TYPE A - Basic hive sensor and from this->sensor's address
          Serial.println("got valid response");
          parseCSV();
        }
        delay(CHKDELAY);
      } 
      
      
    }
    else{
      Serial.println(F("[ ER ] poll req. send"));
      return -1;
    }
  }
  
};
