#include "time.h"
#include <sys/time.h>

class ESPtime {
#define TZ_INFO "MST7MDT6,M3.2.0/02:00:00,M11.1.0/02:00:00"
#define MQTT_SEND_PERIOD_MINS 5
#define SENS_UPDATE_PERIOD_MINS 3
#define UPDATE_TIME_PERIOD_MINS 13
  private:
    const int32_t gmtOffset_sec = 3600*2;       /// CET GMT+2
    //const int   daylightOffset_sec = 3600;    /// I refuse to use it.
    const char* ntpServer = "pool.ntp.org";
    /*SIM7000 modem;*/
    /*bool GPStime = false;*/

    bool timeUpdated = -1;

    int32_t lastMqttSend = 0;

    struct tm now;

  public:

    ESPtime() {
      setenv("TZ", TZ_INFO, 1);
      tzset();

      // configTime(gmtOffset_sec, daylightOffset_sec);
    };
   /* void useModem(SIM7000 &gsmModem) {
      this->modem = gsmModem;
      this->GPStime = true;
    }*/

    int setUnixtime(int32_t unixtime) {
      timeval epoch = {unixtime, 0};
      return settimeofday((const timeval*)&epoch, 0);
    }

    int8_t updateTimeIfNeeded(bool force = false) {
      static int32_t lastUpdate = 0;
      if(!force && abs((int32_t)time(NULL) - lastUpdate) < (UPDATE_TIME_PERIOD_MINS*60)){
        return 1;
      }
      DebugSerial.print(F("TIME| system time update."));

      if(simModem.simformation.gnss_unixTime <= 1718222292){
        return -1;
      }

      DebugSerial.print(F("TIME| Old:"));
      printLocalTime();
      setUnixtime((simModem.simformation.gnss_unixTime + gmtOffset_sec));
      lastUpdate = (int32_t)time(NULL);
      DebugSerial.print(F("TIME| New:"));
      printLocalTime();
      DebugSerial.println(F("TIME| [ OK ]"));
      return 0;
      
    }

    void printLocalTime()
    {
      getLocalTime(&now, 0);
      DebugSerial.print(&now, " %B %d %Y %H:%M:%S (%A)\n");
    }

    bool isTimeForSend(){
      if(abs((int32_t)time(NULL) - lastMqttSend) < (MQTT_SEND_PERIOD_MINS*60)){
        return false;
      }
      getLocalTime(&now, 0);
      if((now.tm_min % 5) == 0){
        DebugSerial.println("Send it.");
        lastMqttSend = (int32_t)time(NULL);
        return true;
      }
      return false; 
    }

    bool isTimeForSensorUpdate(){
      if(abs((int32_t)time(NULL) - lastMqttSend) < (SENS_UPDATE_PERIOD_MINS*60)){
        return false;
      }
      return true;
    }

    /*bool isTimeForESPReboot(){
      static int8_t prevMin = 0;
      bool isRebootTime = false;
      
      getLocalTime(&now, 0);
      if((now.tm_hour == 0) && (now.tm_min == 22) && (((now.tm_min - prevMin) == 1) || (abs((now.tm_min - prevMin)) == 59 ))&& (lastRebootReason != 3)){
        DebugSerial.println("Send it.");
        lastMqttSend = (int32_t)time(NULL);
        isRebootTime = true;
      }

      prevMin = now.tm_min;
      return isRebootTime; 
    }*/
};
