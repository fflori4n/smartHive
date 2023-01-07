#include <SoftwareSerial.h> /// probably will be removed
#include "LowPower.h"
#include "DHT.h"
#include <avr/power.h>
#include <avr/sleep.h>


#define RO_PIN 5//10
#define DI_PIN 3//11
#define DE_PIN 4//12
#define DHT0_PIN 10
#define DHT1_PIN 11
#define DHT2_PIN 12
#define WAKE_PIN 2
#define PIR_PIN 9
#define GPIO0_PIN 6
#define GPIO1_PIN 7
#define GPIO2_PIN 8
//#define GPIO3_PIN 9

#define DHT0_TYPE DHT22
#define DHT1_TYPE DHT11
#define DHT2_TYPE DHT11

#define DHT_TYPE DHT11   // DHT 11
DHT dht0(DHT0_PIN, DHT0_TYPE);
DHT dht1(DHT1_PIN, DHT1_TYPE);// 8
DHT dht2(DHT2_PIN, DHT2_TYPE);// 7

int dht0Temp = -999;  /// int representation of float value, truncated to 1 decimal, and multiplied by 10, would be -99.9
int dht0Humi = -999;
int dht1Temp = -999;
int dht1Humi = -999;
int dht2Temp = -999;
int dht2Humi = -999;

//unsigned int loopCount = 0;

volatile unsigned long timerSeconds = 999;
volatile unsigned long secondsSincePoll = 0;
bool noComError = false;


uint8_t tiltSensors = 0x00;
uint8_t pIRsensors = 0x00;

#include "RS485Com.h"
SoftwareSerial Serial485 (RO_PIN, DI_PIN); // RX, TX
RS485Com serial;

void setup() {
  Serial.begin(9600); 
  dht0.begin();
  dht1.begin();
  dht2.begin();

  pinMode(WAKE_PIN, INPUT_PULLUP);
  pinMode(PIR_PIN,  INPUT_PULLUP);

  // disable ADC
  ADCSRA = 0;   

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

/* Now is the time to set the sleep mode. In the Atmega8 datasheet
 * http://www.atmel.com/dyn/resources/prod_documents/doc2486.pdf on page 35
 * there is a list of sleep modes which explains which clocks and
 * wake up sources are available in which sleep modus.
 *
 * In the avr/sleep.h file, the call names of these sleep modus are to be found:
 *
 * The 5 different modes are:
 * SLEEP_MODE_IDLE -the least power savings
 * SLEEP_MODE_ADC
 * SLEEP_MODE_PWR_SAVE
 * SLEEP_MODE_STANDBY
 * SLEEP_MODE_PWR_DOWN -the most power savings
 *
 * the power reduction management <avr/power.h> is described in
 * http://www.nongnu.org/avr-libc/user-manual/group_avr_power.html
 */

set_sleep_mode(SLEEP_MODE_IDLE); // sleep mode is set here

sleep_enable(); // enables the sleep bit in the mcucr register
// so sleep is possible. just a safety pin

power_adc_disable();
power_spi_disable();
power_timer0_disable();
//power_timer1_disable();
power_timer2_disable();
power_twi_disable();
sleep_mode(); // here the device is actually put to sleep!!
// THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP
sleep_disable(); // first thing after waking from sleep:
// disable sleep...

power_all_enable();

}

ISR( TIMER1_COMPA_vect ) 
{
  timerSeconds++;
  secondsSincePoll++;
}

/*ISR( PCINT2_vect ){         TODO: pin change interrupts conflict with softwareserial, maybe switch to alt sofrware serial?
  
}*/
/*ISR (PCINT0_vect)
{
  //Serial.print("pir triggered!");
}*/

void readDHT(DHT &dhtSensor, int &temp, int &humidity){  /// TODO: optimise this.
  #define PRINT_READINGS
  #define MAX_DHT_TEMP 99
  #define MIN_DHT_TEMP -99
  #define MAX_DHT_HUMI 100
  #define MIN_DHT_HUMI 0
  #define RETRY_DHT_READ 2
  #define AVERAGE_FILTER_NEW_WEIGHT 0.5

  double newTemp = -999;
  double newHumi = -999;
  byte updatedFlag;

    updatedFlag = 0;
    for(int i=0; i < RETRY_DHT_READ; i++){
      newTemp = dhtSensor.readTemperature();
      newHumi = dhtSensor.readHumidity();
      
      Serial.println(newTemp);
      Serial.println(newHumi);
      if((updatedFlag != 1) && !isnan(newTemp) && (newTemp <= MAX_DHT_TEMP) && (newTemp >= MIN_DHT_TEMP)){
        if(temp != -999){
           temp = (int)((temp * (1-AVERAGE_FILTER_NEW_WEIGHT)) + (newTemp*10)*AVERAGE_FILTER_NEW_WEIGHT);
        }
        else{
          temp = (int)(newTemp*10);
        }
       
        updatedFlag+=1;
      }
      if((updatedFlag != 2) && !isnan(newHumi) && (newTemp <= MAX_DHT_HUMI) && (newTemp >= MIN_DHT_HUMI)){ 
        if(humidity != -999){
           humidity = (int)((humidity * (1-AVERAGE_FILTER_NEW_WEIGHT)) + (newHumi*10)*AVERAGE_FILTER_NEW_WEIGHT);
        }
        else{
          humidity = (int)(newHumi*10);
        }
        updatedFlag+=2;
      }
      
      if(updatedFlag >=3){
        #ifdef PRINT_READINGS
        Serial.print(F("DHT|Temp: "));
        Serial.print(newTemp);
        Serial.print(F(" Humidity: "));
        Serial.println(newHumi);
        #endif
        break;
      }
      //delay(20);
    }
    if(updatedFlag < 3){
      Serial.println(F("Could not update temp & humidity."));
    }
}

void loop() {
 // Serial.println(F("Sleep"));
  goIdle();
 // Serial.println(F("Woke up."));

  if(digitalRead(PIR_PIN) == HIGH){         /// poll for now, maybe don't need the interrupt
    pIRsensors = 1;
    //Serial.print("motion detected!");
  }
  if(timerSeconds > 60){
      Serial.println(F("reading DHT 60 sec"));
      readDHT(dht0, dht0Temp, dht0Humi);
      readDHT(dht1, dht1Temp, dht1Humi);
      readDHT(dht2, dht2Temp, dht2Humi);
      timerSeconds = 0;
  }
  while(serial.isAvailable() == 0){        
    //delay(200);
    if(serial.checkInbuf() == 0 && serial.chkIfPoll() == 0){
      serial.respond2Poll();
      noComError = false;
      secondsSincePoll = 0;
      pIRsensors = 0;
    }
    delay(20);
  }
  if(secondsSincePoll > 30){
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
