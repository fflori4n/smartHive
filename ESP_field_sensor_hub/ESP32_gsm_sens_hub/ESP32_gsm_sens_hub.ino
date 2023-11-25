#define _TEMP_STRLEN 200
#define MQTT_PAYLOAD_BUFF_LEN 1000

#include "settings_globaldefinitions.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

/** One wire temperature sensors */
OneWire oneWire(HUBDS18B20_ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress hub_temp0, hub_temp1;

#include <esp_task_wdt.h>
#include "RS485Com.h"
#include "BasicHiveSensor.h"
#include "sim7000.h"
#include "sensHubInfo.h"
#include "ReadVoltage.h"
#include "SolarLogger.h"
#include "wifiFunctions.h"
#include "Mqtt.h"
#include "ESPtime.h"

#define SENS_PWR_ON 23
#define SENS_PWR_OFF 22

char mqttPayloadBuffer[MQTT_PAYLOAD_BUFF_LEN] = "{";
char mqttPayloadBuff[MQTT_PAYLOAD_BUFF_LEN] = "";
char tempStrBuffer[_TEMP_STRLEN];

bool sensorsUpToDate = false;
bool initialOnlineReport = false;

SIM7000 gsmModem = SIM7000();
RS485Com com485 = RS485Com();

BasicHiveSensor sensorA((char)65,"BHS_A", com485);
BasicHiveSensor sensorB((char)66,"BHS_B", com485);


BasicHiveSensor* bHSensList[2] = {&sensorA, &sensorB};
ESPtime hubTime;

bool lowBattery = false;




void print_reset_reason(uint8_t reason)
{
  switch (reason)
  {
    case 1 : Serial.println ("POWERON_RESET");break;          /**<1, Vbat power on reset*/
    case 3 : Serial.println ("SW_RESET");break;               /**<3, Software reset digital core*/
    case 4 : Serial.println ("OWDT_RESET");break;             /**<4, Legacy watch dog reset digital core*/
    case 5 : Serial.println ("DEEPSLEEP_RESET");break;        /**<5, Deep Sleep reset digital core*/
    case 6 : Serial.println ("SDIO_RESET (WDT)");break;             /**<6, Reset by SLC module, reset digital core*/
    case 7 : Serial.println ("TG0WDT_SYS_RESET");break;       /**<7, Timer Group0 Watch dog reset digital core*/
    case 8 : Serial.println ("TG1WDT_SYS_RESET");break;       /**<8, Timer Group1 Watch dog reset digital core*/
    case 9 : Serial.println ("RTCWDT_SYS_RESET");break;       /**<9, RTC Watch dog Reset digital core*/
    case 10 : Serial.println ("INTRUSION_RESET");break;       /**<10, Instrusion tested to reset CPU*/
    case 11 : Serial.println ("TGWDT_CPU_RESET");break;       /**<11, Time Group reset CPU*/
    case 12 : Serial.println ("SW_CPU_RESET");break;          /**<12, Software reset CPU*/
    case 13 : Serial.println ("RTCWDT_CPU_RESET");break;      /**<13, RTC Watch dog Reset CPU*/
    case 14 : Serial.println ("EXT_CPU_RESET");break;         /**<14, for APP CPU, reseted by PRO CPU*/
    case 15 : Serial.println ("RTCWDT_BROWN_OUT_RESET");break;/**<15, Reset when the vdd voltage is not stable*/
    case 16 : Serial.println ("RTCWDT_RTC_RESET");break;      /**<16, RTC Watch dog reset digital core and rtc module*/
    default : Serial.println ("NO_MEAN");
  }
}
void setup()  
{
  /* This is not conventional, but the most important part is communication, so to avoid error i set the WDT for msg sending period*/
  #define WDT_TIMEOUT (uint16_t)((60 * 5)*1.5)
  /* start WDT */
  esp_task_wdt_init(WDT_TIMEOUT, true); //enable panic so ESP32 restarts
  esp_task_wdt_add(NULL); //add current thread to WDT watch
  
//  simModem.begin(15200, SERIAL_8N1, 16, 17); 
  Serial.begin(115200);
  Serial.println("DBG Serial [ OK ]");
  lastRebootReason = esp_reset_reason();
  print_reset_reason(lastRebootReason);

  setupDS18B20(sensors);
  readDS18B20(sensors);
  
  gsmModem.init();
  hubTime.useModem(gsmModem);
  solarLogSerial.begin(4800);
  delay(2000);

  /// pin for reading voltages
  pinMode(VNPIN, INPUT);
  analogSetWidth(11);
  analogReadResolution(11);

  pinMode(SENS_PWR_ON, OUTPUT);
  pinMode(SENS_PWR_OFF, OUTPUT);
  digitalWrite(SENS_PWR_ON, LOW);
  digitalWrite(SENS_PWR_OFF, LOW);
  
  setSensorPwr(true);
  hubTime.updateTimeIfNeeded(true);
} 

void setSensorPwr(bool isON){
  if(isON){
    digitalWrite(SENS_PWR_ON, HIGH);
    delayMicroseconds(100);
    digitalWrite(SENS_PWR_ON, LOW);
  }
  else{
    digitalWrite(SENS_PWR_OFF, HIGH);
    delay(10);
    digitalWrite(SENS_PWR_OFF, LOW);
  }
}

void loop()  
{ 
  readVoltages();

  bool powerSavingEnabled = isPowerSavingEnabled();
  //Serial.println();
  if(!sensorsUpToDate && hubTime.isTimeForSensorUpdate()){
    if(!powerSavingEnabled){
      setSensorPwr(true);
      delay(20000);
      sensorA.update();
      sensorB.update();
      setSensorPwr(false);
    }
    sensorsUpToDate = true;
  }
  else if(hubTime.isTimeForSend() || !initialOnlineReport){
    Serial.println("MQTT SEND:");
    sendMqttStatusMsg(gsmModem,mqttPayloadBuff, "RTU0/RTU_INFO");
    esp_task_wdt_reset();
    delay(1000);
    if(!powerSavingEnabled){
      sendMqttBHSensorMsg(gsmModem,mqttPayloadBuff,"RTU0/BHSENS", bHSensList, 2);
      
    }
    sensorsUpToDate = false;
    initialOnlineReport = true;
  }
  //hubTime.printLocalTime();
  hubTime.updateTimeIfNeeded();

  if(readyForReboot){
    Serial.println("rebooting ESP!");
    ESP.restart();
  }
  delay(5000);
}
