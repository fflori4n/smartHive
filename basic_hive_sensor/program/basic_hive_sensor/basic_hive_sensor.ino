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
#define GPIO0_PIN 6
#define GPIO1_PIN 7
#define GPIO2_PIN 8
#define GPIO3_PIN 9

#define DHT_TYPE DHT11   // DHT 11
DHT dht0(DHT0_PIN, DHT_TYPE);
DHT dht1(DHT1_PIN, DHT_TYPE);// 8
DHT dht2(DHT2_PIN, DHT_TYPE);// 7

int dht0Temp = -999;  /// int representation of float value, truncated to 1 decimal, and multiplied by 10, would be -99.9
int dht0Humi = -999;
int dht1Temp = -999;
int dht1Humi = -999;
int dht2Temp = -999;
int dht2Humi = -999;

unsigned int loopCount = 0;

unsigned int tiltSensors = 0x00;
unsigned int pIRsensors = 0x00;

#include "RS485Com.h"
SoftwareSerial Serial485 (RO_PIN, DI_PIN); // RX, TX
RS485Com serial;

void setup() {
  Serial.begin(9600); 
  dht0.begin();
  dht1.begin();
  dht2.begin();

  pinMode(WAKE_PIN, INPUT); 

  // disable ADC
  ADCSRA = 0;   

  /*/// enable interrupt on pin2
  DDRD  &= 0b11111011;   // set PD2 to input 
  PORTD |= 0b00000100;   // set PD2 to high
 
  PCICR |= _BV(PCIE2);   //Enable PCINT2
  PCMSK2 |= _BV(PCINT18); //Trigger on change of PCINT18 (PD2)
  sei(); */
}

/*ISR(PCINT2_vect ) 
{
    Serial.print("interrupt - TX incoming...");
}*/

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
power_timer1_disable();
power_timer2_disable();
power_twi_disable();

sleep_mode(); // here the device is actually put to sleep!!

// THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP
sleep_disable(); // first thing after waking from sleep:
// disable sleep...

power_all_enable();

}
void readDHTs(){  /// TODO: optimise this.
  #define PRINT_READINGS
    dht0Temp = -999;
    dht1Temp = -999;
    dht2Temp = -999;

    dht0Humi = -999;
    dht1Humi = -999;
    dht2Humi = -999;

    double newTemp = dht0.readTemperature();
    double newHumi = dht0.readHumidity();
    //delay(2000);
    if(!isnan(newTemp)){
      dht0Temp = (int)(newTemp*10);
    }
    if(!isnan(newTemp)){
      dht0Humi = (int)(newHumi*10);
    }
    #ifdef PRINT_READINGS
    Serial.print(F("DHT0|Temp: "));
    Serial.print(newTemp);
    Serial.print(F(" Himidity: "));
    Serial.println(newHumi);
    #endif

    newTemp = dht1.readTemperature();
    newHumi = dht1.readHumidity();
    //delay(2000);
    if(!isnan(newTemp)){
      dht1Temp = (int)(newTemp*10);
    }
    if(!isnan(newTemp)){
      dht1Humi = (int)(newHumi*10);
    }
    #ifdef PRINT_READINGS
    Serial.print(F("DHT1|Temp: "));
    Serial.print(newTemp);
    Serial.print(F(" Himidity: "));
    Serial.println(newHumi);
    #endif

    newTemp = dht2.readTemperature();
    newHumi = dht2.readHumidity();
    //delay(2000);
    if(!isnan(newTemp)){
      dht2Temp = (int)(newTemp*10);
    }
    if(!isnan(newTemp)){
      dht2Humi = (int)(newHumi*10);
    }
    #ifdef PRINT_READINGS
    Serial.print(F("DHT2|Temp: "));
    Serial.print(newTemp);
    Serial.print(F(" Himidity: "));
    Serial.println(newHumi);
    #endif
}
void ISR_TX(){
 // Serial.print(F());
 /* if(serial.checkInbuf() == 0 && serial.chkIfPoll() == 0){
    serial.respond2PollAll();
  }*/
}
void loop() {
  if(serial.checkInbuf() == 0 && serial.chkIfPoll() == 0){
    readDHTs();
    serial.respond2Poll();
  }
  delay(10);

  if (loopCount > 250) {
    goIdle();
    Serial.println(F("Woke up."));
    
    //Serial.println(F("loop"));
    loopCount = 0;
    delay(200);
    //Serial.println(RAMEND - SP);
  }
  loopCount ++;
}
