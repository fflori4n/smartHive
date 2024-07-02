#define _TEMP_STRLEN 200
#define MQTT_PAYLOAD_BUFF_LEN 1000

#include "settings_globaldefinitions.h"
#include <WiFi.h>
#include <HardwareSerial.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

/** One wire temperature sensors */
OneWire oneWire(HUBDS18B20_ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress hub_temp0, hub_temp1;

HardwareSerial Serial485(0);
HardwareSerial GSMserial(1);
HardwareSerial DebugSerial(2);

#include <HardwareSerial.h>
#include <esp_task_wdt.h>
#include "RS485Com.h"
#include "BasicHiveSensor.h"
#include "sim7000.h"

#include "ReadVoltage.h"
#include "SolarLogger.h"
#include "wifiFunctions.h"

SimModem simModem(GSMserial);

#include "sensHubInfo.h"
#include "Mqtt.h"
#include "ESPtime.h"

#define SENS_PWR_ON 23
#define SENS_PWR_OFF 22

char mqttPayloadBuffer[MQTT_PAYLOAD_BUFF_LEN] = "{";
char mqttPayloadBuff[MQTT_PAYLOAD_BUFF_LEN] = "";
char tempStrBuffer[_TEMP_STRLEN];

bool sensorsUpToDate = false;
bool initialOnlineReport = false;

RS485Com com485 = RS485Com();

BasicHiveSensor sensorA((char)65, "BHS_A", com485);
BasicHiveSensor sensorB((char)66, "BHS_B", com485);


BasicHiveSensor* bHSensList[2] = {&sensorA, &sensorB};
ESPtime hubTime;

bool lowBattery = false;

uint8_t resetSimModem = 0;




void print_reset_reason(uint8_t reason)
{
  switch (reason)
  {
    case 1 : DebugSerial.println ("POWERON_RESET"); break;         /**<1, Vbat power on reset*/
    case 3 : DebugSerial.println ("SW_RESET"); break;              /**<3, Software reset digital core*/
    case 4 : DebugSerial.println ("OWDT_RESET"); break;            /**<4, Legacy watch dog reset digital core*/
    case 5 : DebugSerial.println ("DEEPSLEEP_RESET"); break;       /**<5, Deep Sleep reset digital core*/
    case 6 : DebugSerial.println ("SDIO_RESET (WDT)"); break;            /**<6, Reset by SLC module, reset digital core*/
    case 7 : DebugSerial.println ("TG0WDT_SYS_RESET"); break;      /**<7, Timer Group0 Watch dog reset digital core*/
    case 8 : DebugSerial.println ("TG1WDT_SYS_RESET"); break;      /**<8, Timer Group1 Watch dog reset digital core*/
    case 9 : DebugSerial.println ("RTCWDT_SYS_RESET"); break;      /**<9, RTC Watch dog Reset digital core*/
    case 10 : DebugSerial.println ("INTRUSION_RESET"); break;      /**<10, Instrusion tested to reset CPU*/
    case 11 : DebugSerial.println ("TGWDT_CPU_RESET"); break;      /**<11, Time Group reset CPU*/
    case 12 : DebugSerial.println ("SW_CPU_RESET"); break;         /**<12, Software reset CPU*/
    case 13 : DebugSerial.println ("RTCWDT_CPU_RESET"); break;     /**<13, RTC Watch dog Reset CPU*/
    case 14 : DebugSerial.println ("EXT_CPU_RESET"); break;        /**<14, for APP CPU, reseted by PRO CPU*/
    case 15 : DebugSerial.println ("RTCWDT_BROWN_OUT_RESET"); break; /**<15, Reset when the vdd voltage is not stable*/
    case 16 : DebugSerial.println ("RTCWDT_RTC_RESET"); break;     /**<16, RTC Watch dog reset digital core and rtc module*/
    default : DebugSerial.println ("NO_MEAN");
  }
}
void setup()
{
  /* This is not conventional, but the most important part is communication, so to avoid error i set the WDT for msg sending period*/
#define WDT_TIMEOUT (uint16_t)((60 * 5)*1.5)
  /* start WDT */
  esp_task_wdt_init(WDT_TIMEOUT, true); //enable panic so ESP32 restarts
  esp_task_wdt_add(NULL); //add current thread to WDT watch

  DebugSerial.begin(115200, SERIAL_8N1, 3, 1);
  DebugSerial.println("DBG Serial [ OK ]");
  lastRebootReason = esp_reset_reason();
  print_reset_reason(lastRebootReason);

  setupDS18B20(sensors);
  readDS18B20(sensors);
  //
  int8_t er = simModem.begin(9600);
  DebugSerial.println(er);

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

  simModem.setBaseConfig();

  // gsmModem.atPrint("AT+CPSMS?\r","OK",500);
  //gsmModem.atPrint("AT+CPSMRDP\r","OK",500);
  //gsmModem.atSend("AT+CPSMS=0,\"01100000\",\"00000000\",\"00000001\",\"01011100\"\r", "OK", 500);/* go into PSM mode for 10 mins*/
  //gsmModem.atSend("AT+CPSMS=0\r","OK",15000);

  //gsmModem.atPrint("AT+CPSMRDP\r","OK",500);
  
  DebugSerial.println("485 Serial was started [ OK ]");
}

void sendPowerCycleReq() {
  pinMode(14, OUTPUT);
  delay(500);
  for (int i = 0; i < 5; i++) {
    digitalWrite(14, HIGH);
    delay(500);
    digitalWrite(14, LOW);
    delay(1000);
  }
}
void setSensorPwr(bool isON) {
  if (isON) {
    digitalWrite(SENS_PWR_ON, HIGH);
    delayMicroseconds(100);
    digitalWrite(SENS_PWR_ON, LOW);
  }
  else {
    digitalWrite(SENS_PWR_OFF, HIGH);
    delay(10);
    digitalWrite(SENS_PWR_OFF, LOW);
  }
}

void loop()
{
  readVoltages();


  bool powerSavingEnabled = isPowerSavingEnabled();
  
  //    if (mqttMsgId >= (12 * 24)) {
  //      readyForReboot = true;
  //    }
  //
  //    if (resetSimModem >= (6 * 12)) {  /* reset CFUN 6 every 4 hours, for no good reason, but see if it will become more reliable, eventualy hardware power reset should be implemented if modem gets very stuck*/
  //      resetSimModem = 0;
  //      gsmModem.atSend("AT+CFUN=1,1\r", "OK", 15000);
  //      gsmModem.atSend("AT\r", "RDY", 60000);
  //    }
  //    else {
  //      resetSimModem++;
  //    }
  //
  //    /*gsmModem.atSend("AT+CPSMS=1,\"01100000\",\"00000000\",\"00000001\",\"01011100\"\r", "OK", 500); *//* go into PSM mode for 10 mins*/
  //    sensorsUpToDate = false;
  //    initialOnlineReport = true;
  //  }
  //  //hubTime.printLocalTime();
  //  if (0 != hubTime.updateTimeIfNeeded()) { /* 1 - no update needed */
  //      /* reset WDT if time was updated successfully*/
  //  }
  //
  //  
  //  if (powerKeyCycle == true) {
  //    sendPowerCycleReq();
  //  }

  if (!sensorsUpToDate && hubTime.isTimeForSensorUpdate()) {
    
      setSensorPwr(true);
      delay(20000);
      sensorA.update();
      sensorB.update();
      /*setSensorPwr(false);*/
    sensorsUpToDate = true;
  }
  else if (hubTime.isTimeForSend() || !initialOnlineReport) {

    simModem.connectToNetwork();
    simModem.getGNSSData();
    simModem.getRSSI();
    if (0 != hubTime.updateTimeIfNeeded(true)) { /* 1 - no update needed */
      esp_task_wdt_reset(); /* reset WDT if time was updated successfully*/
    }
    sendMqttStatusMsg(mqttPayloadBuff, "RTU0/RTU_INFO");
    vTaskDelay(200 / portTICK_PERIOD_MS);
    sendMqttBHSensorMsg(mqttPayloadBuff, "RTU0/BHSENS", bHSensList, 2);
    vTaskDelay(200 / portTICK_PERIOD_MS);
    simModem.mqttClientDisconnect();

    
    sensorsUpToDate = false;
    initialOnlineReport = true;
  }

  if (mqttMsgId >= (12 * 24)) {
    DebugSerial.println("rebooting ESP!");
    ESP.restart();
  }

  delay(2000);
  
}
