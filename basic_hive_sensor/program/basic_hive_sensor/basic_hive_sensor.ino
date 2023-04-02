#include <SoftwareSerial.h> /// probably will be removed
#include "DHT.h"
#include <avr/power.h>
#include <avr/sleep.h>


#define RO_PIN 5//10
#define DI_PIN 3//11
#define DE_PIN 4//12
#define DHT0_PIN 12
#define DHT1_PIN 11
#define DHT2_PIN 10
#define WAKE_PIN 2
#define PIR_PIN 9
#define GPIO0_PIN 6
#define GPIO1_PIN 7
#define GPIO2_PIN 8
//#define GPIO3_PIN 9
#define ER_NOPOLL_SECONDS (60*15) /// 15mins
#define DHTREAD_PERIOD_SECS 60
#define MOTION_ACTIVATE_PROP 0.1 /// if motion is detected for 10% of DHTREAD PERIOD, motion is detected

#define READGPIO9 (PINB  & B00000010)

#define DHT0_TYPE DHT22
#define DHT1_TYPE DHT22
//#define DHT2_TYPE DHT22

#ifdef DHT0_TYPE
  DHT dht0(DHT0_PIN, DHT0_TYPE);
#endif
#ifdef DHT1_TYPE
  DHT dht1(DHT1_PIN, DHT1_TYPE);
#endif
#ifdef DHT2_TYPE
  DHT dht2(DHT2_PIN, DHT2_TYPE);
#endif

volatile unsigned long timerSeconds = 999;
volatile unsigned long secondsSincePoll = 0;
volatile unsigned long motionCount=0;

uint8_t tiltSensors = 0x00;
uint8_t pIRsensors = 0;

int dht0Temp = -999;  /// int representation of float value, truncated to 1 decimal, and multiplied by 10, would be -99.9
int dht0Humi = -999;
int dht1Temp = -999;
int dht1Humi = -999;
int dht2Temp = -999;
int dht2Humi = -999;

bool noComError = false;




#include "RS485Com.h"
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
}
void goIdle(){ /// From: https://rubenlaguna.com/post/2008-10-15-arduino-sleep-mode-waking-up-when-receiving-data-on-the-usart/

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

  if(READGPIO9){                /// Softserial uses all pinchange interrupts, so do polling of motion sens. every sec - not optimal but whatever
     motionCount++;
  }
}

void readDHT(DHT &dhtSensor, int &temp, int &humidity){  /// TODO: optimise this.
  #define PRINT_READINGS
  #define MAX_DHT_TEMP 99
  #define MIN_DHT_TEMP -99
  #define MAX_DHT_HUMI 100
  #define MIN_DHT_HUMI 0
  #define RETRY_DHT_READ 10
  #define AVERAGE_FILTER_NEW_WEIGHT 0.5

  double newTemp = -999;
  double newHumi = -999;
  byte updatedFlag;

    updatedFlag = 0;
    for(int i=0; i < RETRY_DHT_READ; i++){
      newTemp = dhtSensor.readTemperature();
   
      if(!isnan(newTemp) && (newTemp <= MAX_DHT_TEMP) && (newTemp >= MIN_DHT_TEMP)){
        if(temp != -999){
           temp = (int)((temp * (1-AVERAGE_FILTER_NEW_WEIGHT)) + (newTemp*10)*AVERAGE_FILTER_NEW_WEIGHT);
        }
        else{
          temp = (int)(newTemp*10);
        }
       
        updatedFlag+=1;
        break;
      }
      delay(1000);
    }
    for(int i=0; i < RETRY_DHT_READ; i++){
      newHumi = dhtSensor.readHumidity();
      if(!isnan(newHumi) && (newTemp <= MAX_DHT_HUMI) && (newTemp >= MIN_DHT_HUMI)){ 
        if(humidity != -999){
           humidity = (int)((humidity * (1-AVERAGE_FILTER_NEW_WEIGHT)) + (newHumi*10)*AVERAGE_FILTER_NEW_WEIGHT);
        }
        else{
          humidity = (int)(newHumi*10);
        }
        updatedFlag+=2;
        break;
      }
      delay(1000);
    }

    if(updatedFlag >=3){
      
    }
    
    #ifdef PRINT_READINGS
    switch(updatedFlag){
      case 3:
        Serial.print(F("DHT|Temp: "));
        Serial.print(newTemp);
        Serial.print(F(" Humidity: "));
        Serial.println(newHumi);
        break;
      case 2:
        Serial.print(F("DHT|Temp: "));
        Serial.print(newTemp);
        Serial.print(F(" Humidity: x"));
        break;
      case 1:
        Serial.print(F("DHT|Temp: x"));
        Serial.print(F(" Humidity: "));
        Serial.println(newHumi);
        break;
      case 0:
        Serial.print(F("DHT|Temp: x"));
        Serial.print(F(" Humidity: x"));
        break;
    }
    #endif
}

void loop() {
  //Serial.println(F("Sleep"));
  goIdle();
  //Serial.println(F("Woke up."));

  if(timerSeconds > DHTREAD_PERIOD_SECS){
      Serial.println(F("reading DHT 60 sec"));
      #ifdef DHT0_TYPE
      readDHT(dht0, dht0Temp, dht0Humi);
      #endif
      #ifdef DHT1_TYPE
      readDHT(dht1, dht1Temp, dht1Humi);
      #endif
      #ifdef DHT2_TYPE
      readDHT(dht2, dht2Temp, dht2Humi);
      #endif

      if(motionCount > MOTION_ACTIVATE_PROP*timerSeconds){ 
        Serial.print(F("PIR| Detected!"));
        pIRsensors = 1;
      }
      else{
        Serial.print(F("PIR| No motion!"));
      }
      motionCount = 0;
      timerSeconds = 0;
  }
  //noInterrupts();
  while(serial.isAvailable() == 0){        
    if(serial.checkInbuf() == 0 && serial.chkIfPoll() == 0){
      serial.respond2Poll();
      noComError = false;
      secondsSincePoll = 0;
      pIRsensors = 0;
    }
    delay(20);
  }
  //interrupts();
  if(secondsSincePoll > ER_NOPOLL_SECONDS){
    noComError = true;
  }
  if(noComError){
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
