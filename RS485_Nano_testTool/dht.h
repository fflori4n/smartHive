
#define DHTPIN 3     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

/// Using #include "DHT.h" - some built in arduino function or whatever
class DHTSensor{
  private:
  public:
    char* dhtType;
    byte dhtPin;

    double humidity = -1;
    double temp = -999;
    double heatIndex = -999;
    
    DHTSensor(uint8_t dhtPin, char* dhtType){
      this->dhtType = dhtType;
      this->dhtPin = dhtPin;
    }
    void read(){
      DHT dht(dhtPin, dhtType); /// not very efficient! but ok for now

      this->humidity = dht.readHumidity();
      this->temp = dht.readTemperature();

      if (isnan(this->humidity) || isnan(this->temp)) {
        Serial.println(F("Failed to read from DHT sensor!"));
        return;
      }
      this->heatIndex = dht.computeHeatIndex(temp, humidity, false);
    }
    void dbgPrint(){
      Serial.print(F("DHT Sensor:"));
      Serial.print(F("Hum: "));
      Serial.print(this->humidity);
      Serial.print(F(" Temp: "));
      Serial.print(this->temp);
      Serial.print(F(" Hindex: "));
      Serial.println(this->heatIndex);
    }
};
