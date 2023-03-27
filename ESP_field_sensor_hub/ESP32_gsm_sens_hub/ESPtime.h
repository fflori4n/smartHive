#include "time.h"
#include <sys/time.h>

class ESPtime {
#define TZ_INFO "MST7MDT6,M3.2.0/02:00:00,M11.1.0/02:00:00"
  private:
    const long  gmtOffset_sec = 3600;
    const int   daylightOffset_sec = 3600;
    const char* ntpServer = "pool.ntp.org";
    SIM7000 modem;
    bool GPStime = false;

    bool timeUpdated = -1;

  public:

    ESPtime() {
      setenv("TZ", TZ_INFO, 1);
      tzset();

      // configTime(gmtOffset_sec, daylightOffset_sec);
    };
    void useModem(SIM7000 &gsmModem) {
      this->modem = gsmModem;
      this->GPStime = true;
    }

    int setUnixtime(int32_t unixtime) {
      timeval epoch = {unixtime, 0};
      return settimeofday((const timeval*)&epoch, 0);
    }

    void updateTimeIfNeeded() {
      if(!GPStime){
        return;
      }
      int32_t newTime;
      if(modem.getUnixTm(newTime) != 0 ){
        return;
      }
      setUnixtime(newTime);
      Serial.print("new time is: ");
      Serial.println(newTime);
      printLocalTime();
      
      
      /// TODO: check if time was updated, and get new time from SIM7000
    }

    void printLocalTime()
    {
      /*struct tm timeinfo;
        if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
        return;
        }
        Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");*/
      struct tm now;
      getLocalTime(&now, 0);
      if (now.tm_year >= 117) Serial.println(&now, "%B %d %Y %H:%M:%S (%A)");

      Serial.println(&now, "%B %d %Y %H:%M:%S (%A)");
    }
};
