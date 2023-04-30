#ifndef USING_SETTINGS_H_RS485CONF
#define USING_SETTINGS_H_RS485CONF

/// BASIC PINOUT

/// - RS485 PINS - 
#define RO_PIN 5//10
#define DI_PIN 3//11
#define DE_PIN 4//12

/// - DHT SENSORS -
#define DHT0_PIN 10
#define DHT1_PIN 11
#define DHT2_PIN 12

/// - OTHER PINS -
#define WAKE_PIN 2
#define PIR_PIN 9
#define GPIO0_PIN 6
#define GPIO1_PIN 7
#define GPIO2_PIN 8
//#define GPIO3_PIN 9

/// - TM PERIODS AND MACROS -
#define DHTREAD_PERIOD_SECS 60
#define ER_NOPOLL_SECONDS (60*15) /// 15mins
#define WAITS_ON_POWERON 3 /// wait secs when power is turned on for sensors and other peripheries to power on properly, before reading them.

#define MOTION_ACTIVATE_PROP 0.1 /// if motion is detected for 10% of DHTREAD PERIOD, motion is detected
#define MOTION_TRIGGER_MODE 1 /// 0 - Zero tolerance for motion, use this when sensor is using power saving mode, motion detection could still return false results, 1 - normal mode
#define READGPIO9 (PINB  & B00000010)



/// - DHT SENSOR OBJECTS -
/// DEFINE DHT SENSORS USED, AND CREATE DHT TYPE OBJECT - COMMENT/UNCOMMENT and CHANGE DHT TYPE
//#define DHT0_TYPE DHT22
#define DHT1_TYPE DHT22
#define DHT2_TYPE DHT22

#ifdef DHT0_TYPE
  DHT dht0(DHT0_PIN, DHT0_TYPE);
#endif
#ifdef DHT1_TYPE
  DHT dht1(DHT1_PIN, DHT1_TYPE);
#endif
#ifdef DHT2_TYPE
  DHT dht2(DHT2_PIN, DHT2_TYPE);
#endif

/// - RS485 CONFIG -
/// RS485 com speed. Keep it low, because of software serial
#define BAUD 9600
/// Addr can't be 0 because translates to '\0' and f*cks up all kinds of string functions!!!
#define THIS_DEV_ADDR 66
#define MASTER_DEV_ADDR 70
#define DEV_TYPE_CHAR 'A'
//#define DEV_IS_MASTER   /// compile with as master (base), or compile as slave (sensor)

#endif
