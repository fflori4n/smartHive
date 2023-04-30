#include <SoftwareSerial.h> /// probably will be removed

#define DHT_DEBUG

#include "DHT.h"
#include "setup.h"
#include "RS485Com.h"
#include <avr/power.h>
#include <avr/sleep.h>

volatile unsigned long timerSeconds = 0;
volatile unsigned long secondsSincePoll = 0;
volatile unsigned long motionCount = 0;

uint8_t tiltSensors = 0x00;
uint8_t pIRsensors = 0;

int dht0Temp = -999;  /// int representation of float value, truncated to 1 decimal, and multiplied by 10, would be -99.9
int dht0Humi = -999;
int dht1Temp = -999;
int dht1Humi = -999;
int dht2Temp = -999;
int dht2Humi = -999;

bool noComError = false;
bool powerOnCompleted = false;

SoftwareSerial Serial485 (RO_PIN, DI_PIN); // RX, TX
RS485Com serial;

void setup() {
  Serial.begin(115200);
  Serial.println(("DBG_SER| Start."));
#ifdef DHT0_TYPE
  pinMode(DHT0_PIN, INPUT_PULLUP);
  dht0.begin();
#endif
#ifdef DHT1_TYPE
  pinMode(DHT1_PIN, INPUT_PULLUP);
  dht1.begin();
#endif
#ifdef DHT2_TYPE
  pinMode(DHT2_PIN, INPUT_PULLUP);
  dht2.begin();
#endif

  pinMode(WAKE_PIN, INPUT_PULLUP);
  pinMode(PIR_PIN,  INPUT_PULLUP);


  ADCSRA = 0;   // disable ADC to save power
  noInterrupts();

  //PCICR |= _BV(PCIE2);   //Enable PCINT2
  //PCMSK2 |= _BV(PCINT18); //Trigger on change of PCINT18 (PD2)
  PCICR |= (1 << PCIE0);    // set PCIE0 to enable PCMSK0 scan
  PCMSK0 |= (1 << PCINT0);  // set PCINT0 to trigger an interrupt on state change

  // Clear registers
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  // 1 Hz (16000000/((15624+1)*1024))
  OCR1A = 15624;
  // CTC
  TCCR1B |= (1 << WGM12);
  // Prescaler 1024
  TCCR1B |= (1 << CS12) | (1 << CS10);
  // Output Compare Match A Interrupt Enable
  TIMSK1 |= (1 << OCIE1A);
  interrupts();
  powerOnCompleted = false;
}
void goIdle() { /// From: https://rubenlaguna.com/post/2008-10-15-arduino-sleep-mode-waking-up-when-receiving-data-on-the-usart/

  set_sleep_mode(SLEEP_MODE_IDLE); // sleep mode is set here

  sleep_enable(); // enables the sleep bit in the mcucr register
  power_adc_disable();
  power_spi_disable();
  power_timer0_disable();
  //power_timer1_disable(); /// keep timer1 awake to count seconds
  power_timer2_disable();
  power_twi_disable();
  sleep_mode(); // here the device is actually put to sleep!!
  // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP
  sleep_disable(); // first thing after waking from sleep:
  power_all_enable();
}

ISR( TIMER1_COMPA_vect )
{
  timerSeconds++;
  secondsSincePoll++;

  if (READGPIO9) {              /// Softserial uses all pinchange interrupts, so do polling of motion sens. every sec - not optimal but whatever
    motionCount++;
  }
}

void readDHT(DHT &dhtSensor, int &temp, int &humidity) { /// TODO: optimise this.
#define PRINT_READINGS
#define MAX_DHT_TEMP 99
#define MIN_DHT_TEMP -99
#define MAX_DHT_HUMI 100
#define MIN_DHT_HUMI 0
#define RETRY_DHT_READ 10
#define AVERAGE_FILTER_NEW_WEIGHT 1 /// 1 to disable filtering. if sensor works in power saving mode.
  //#define KEEP_VALUES_IF_UPDATE_FAILS
#define PAUSE_BETWEEN_SENS_READS 2500

  double newTemp = -999;
  double newHumi = -999;
  byte updatedFlag = 0;

  for (int i = 0; i < RETRY_DHT_READ; i++) {

    updatedFlag = 0;
    noInterrupts();
    newTemp = dhtSensor.readTemperature();
    newHumi = dhtSensor.readHumidity();
    interrupts();

    if (isnan(newTemp) || isnan(newHumi)) {
      Serial.println(F("Failed to read sensor!"));
      delay(PAUSE_BETWEEN_SENS_READS);
      continue;
    }
    if ((newTemp <= MAX_DHT_TEMP) && (newTemp >= MIN_DHT_TEMP)) {
      updatedFlag += 1;
    }
    if ((newHumi <= MAX_DHT_HUMI) && (newHumi >= MIN_DHT_HUMI)) {
      updatedFlag += 2;
    }
  }

  if (temp != -999 && AVERAGE_FILTER_NEW_WEIGHT != 1) {
    temp = (int)((temp * (1 - AVERAGE_FILTER_NEW_WEIGHT)) + (newTemp * 10) * AVERAGE_FILTER_NEW_WEIGHT);
  }
  else {
    temp = (int)(newTemp * 10);
  }

  if (humidity != -999 && AVERAGE_FILTER_NEW_WEIGHT != 1) {
    humidity = (int)((humidity * (1 - AVERAGE_FILTER_NEW_WEIGHT)) + (newHumi * 10) * AVERAGE_FILTER_NEW_WEIGHT);
  }
  else {
    humidity = (int)(newHumi * 10);
  }

  delay(PAUSE_BETWEEN_SENS_READS);

  switch (updatedFlag) {
    case 3:
#ifdef PRINT_READINGS
      Serial.print(F("DHT|Temp: "));
      Serial.print(newTemp);
      Serial.print(F(" Humidity: "));
      Serial.println(newHumi);
#endif
      break;
    case 2:
#ifdef PRINT_READINGS
      Serial.print(F("DHT|Temp: "));
      Serial.print(newTemp);
      Serial.print(F(" Humidity: x"));
#endif
#ifndef KEEP_VALUES_IF_UPDATE_FAILS
      humidity = -999;
#endif
      break;
    case 1:
#ifdef PRINT_READINGS
      Serial.print(F("DHT|Temp: x"));
      Serial.print(F(" Humidity: "));
      Serial.println(newHumi);
#endif
#ifndef KEEP_VALUES_IF_UPDATE_FAILS
      temp = -999;
#endif
      break;
    case 0:
#ifdef PRINT_READINGS
      Serial.print(F("DHT|Temp: x"));
      Serial.print(F(" Humidity: x"));
#endif
#ifndef KEEP_VALUES_IF_UPDATE_FAILS
      temp = -999;
      humidity = -999;
#endif
      break;
  }
}

void loop() {
  //Serial.println(F("Sleep"));
  if (powerOnCompleted) {
    goIdle();
  }
  //Serial.println(F("Woke up."));


  while (serial.isAvailable() == 0) {
    if (serial.checkInbuf() == 0 && serial.chkIfPoll() == 0) {
      serial.respond2Poll(dht0Temp, dht0Humi, dht1Temp, dht1Humi, dht2Temp, dht2Humi, tiltSensors, pIRsensors);
      noComError = false;
      secondsSincePoll = 0;
      pIRsensors = 0;
    }
    delay(20);
  }
  //interrupts();

  if (!powerOnCompleted) {
    if (timerSeconds > WAITS_ON_POWERON) {
      Serial.println(F("TMR|sens. power rdy."));
      powerOnCompleted = true;
      timerSeconds = DHTREAD_PERIOD_SECS + 1;
    }
    else {
      return;
    }
  }

  if (timerSeconds > DHTREAD_PERIOD_SECS) {
    Serial.println(F("TMR|reading sensors."));
#ifdef DHT0_TYPE
    readDHT(dht0, dht0Temp, dht0Humi);
#endif
#ifdef DHT1_TYPE
    readDHT(dht1, dht1Temp, dht1Humi);
#endif
#ifdef DHT2_TYPE
    readDHT(dht2, dht2Temp, dht2Humi);
#endif

    if ((!MOTION_TRIGGER_MODE && motionCount > 0) || (motionCount > MOTION_ACTIVATE_PROP * timerSeconds)) {
      Serial.print(F("PIR| Detected!"));
      pIRsensors = 1;
    }
    else {
      Serial.print(F("PIR| No motion!"));
    }
    motionCount = 0;
    timerSeconds = 0;
  }
  if (secondsSincePoll > ER_NOPOLL_SECONDS) {
    noComError = true;
  }
  if (noComError) {
    digitalWrite(13, HIGH);
    delay(50);
    digitalWrite(13, LOW);
    delay(50);
    digitalWrite(13, HIGH);
    delay(50);
    digitalWrite(13, LOW);
    delay(50);
    digitalWrite(13, HIGH);
    delay(50);
    digitalWrite(13, LOW);
    delay(50);
  }
}
