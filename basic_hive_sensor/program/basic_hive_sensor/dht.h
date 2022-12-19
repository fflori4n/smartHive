#define DHTPIN 3     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

void readDHT(){
  
}
/// Using #include "DHT.h" - some built in arduino function or whatever
class DHTSensor{
  private:
  public:
    char* dhtType;
    byte dhtPin;

    double humidity = -99;
    double temp = -99;
    
    DHTSensor(uint8_t dhtPin, char* dhtType){
      this->dhtType = dhtType;
      this->dhtPin = dhtPin;
    }
    void read(){
      

      double humidity = dht.readHumidity();
      double temp = dht.readTemperature();

      if(!isnan(temp)){
         this->temp = temp;
      }
      else{
        temp = -99;
      }
      if(!isnan(humidity)){
        this->humidity = humidity;
      }
      else{
        humidity = -99;
      }
      
      if (isnan(this->humidity) || isnan(this->temp)) {
        Serial.println(F("Failed to read DHT sensor!"));
        return;
      }


      
    }
    void dbgPrint(){
      Serial.print(F("DHT Sensor:"));
      Serial.print(F("Hum: "));
      Serial.print(this->humidity);
      Serial.print(F(" Temp: "));
      Serial.println(this->temp);
    }
};
