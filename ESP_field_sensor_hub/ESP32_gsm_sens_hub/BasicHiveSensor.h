class BasicHiveSensor{
  private:
  char sensorAddressChar;
  RS485Com serial;
  
  public:
  
  BasicHiveSensor(const char &address,RS485Com &serial){
    this->serial = serial;
    if(address == '\0'){
      Serial.println(F("Sensor can't have address 0"));
    }
    sensorAddressChar = address;
  }

  int readSensor(){
    serial.pollSensor(sensorAddressChar,(char)70);
    delay(2000);
    serial.checkInbuf();
  }
  
};
