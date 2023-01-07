class BasicHiveSensor{
  #define CHKDELAY 500
  
  private:
  char sensorAddressChar;
  char rawResponseBuff[100];
  
  RS485Com &serial;
  uint8_t readTimeOut;

  //double dhtCsvData[] = { -99.9, -99.9, -99.9, -99.9, -99.9, -99.9, 0, 0};
  //const char* labels = {"T0", "H0", "T1", "H1", "T2"}

  void parseCSV(){
    //uint8_t len = serial.getMSGLen();
    char c = serial.get(0);                                       /// !! very cursed - TODO: maybe?
    for(int i=0 ; c != '\0'; c = serial.get(++i)){
      Serial.print(c);
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
