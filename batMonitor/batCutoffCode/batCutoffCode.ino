#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/sleep.h>

#define PWRKEY_PIN 9
#define PWR_CYCLE_REQUEST_PIN 3
#define VSENS_PIN A2

#define TURN_ON_DELAY 10000
#define AWAKE_TIME 10000
#define CYCLE_TIME 200

#define ADC_TO_VOLTAGE (12.48/788)

#define DEBUG_EN 

double voltageFiltered = -1;
double voltageRaw = 0;
uint16_t adcRawValue = 0;

uint16_t turnOnTimer = 0;
uint16_t goToSleepTimer = 0;

void setup() {
  // put your setup code here, to run once:
 // wdt_enable(WDTO_8S);
 // wdt_reset();
#ifdef DEBUG_EN
  Serial.begin(9600);
  Serial.println("Serial OK");
#endif
  pinMode(PWRKEY_PIN, OUTPUT);
  digitalWrite(PWRKEY_PIN, LOW);

//noInterrupts();

  //PCICR |= _BV(PCIE2);   //Enable PCINT2
  //PCMSK2 |= _BV(PCINT18); //Trigger on change of PCINT18 (PD2)
  //PCICR |= (1 << PCIE0);    // set PCIE0 to enable PCMSK0 scan
 // PCMSK0 |= (1 << PCINT0);  // set PCINT0 to trigger an interrupt on state change

  // Clear registers
  //TCCR1A = 0;
 // TCCR1B = 0;
 // TCNT1 = 0;

  // 1 Hz (16000000/((15624+1)*1024))
 // OCR1A = 52082;
  // CTC
  //TCCR1B |= (1 << WGM12);
  // Prescaler 1024
 // TCCR1B |= (1 << CS12) | (1 << CS10);
  // Output Compare Match A Interrupt Enable
  //TIMSK1 |= (1 << OCIE1A);
  //interrupts();
}

void goIdle() { /// From: https://rubenlaguna.com/post/2008-10-15-arduino-sleep-mode-waking-up-when-receiving-data-on-the-usart/

  //wdt_reset();
  //wdt_disable();

 // set_sleep_mode(SLEEP_MODE_IDLE); // sleep mode is set here

 // sleep_enable(); // enables the sleep bit in the mcucr register
 // power_adc_disable();
 // power_spi_disable();
  //power_timer0_disable();
  //power_timer1_disable(); /// keep timer1 awake to count seconds
  //power_timer2_disable();
//  power_twi_disable();
 // sleep_mode(); // here the device is actually put to sleep!!
  // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP
 // sleep_disable(); // first thing after waking from sleep:
//  power_all_enable();
  delay(10000);

//Serial.begin(9600);
Serial.println("Woke up");

 // wdt_enable(WDTO_8S);
  //wdt_reset();
}

int8_t detectPowerCycleRequestPulse(){
  static boolean pulseStarted = false;
  static uint16_t pulseTimeMs = 0;

  int8_t pulseDetected = -1;

  if(false == pulseStarted){
    if(true == digitalRead(PWR_CYCLE_REQUEST_PIN)){
      pulseStarted = true;
    }
  }
  else{
    pulseTimeMs += CYCLE_TIME;

     if(false == digitalRead(PWR_CYCLE_REQUEST_PIN)){
      if((pulseTimeMs >= 100) && (pulseTimeMs <= 1500)){
        pulseDetected = 0;
       }
      pulseTimeMs = 0;
      pulseStarted = false;
    }
  }

  return pulseDetected;
}

void loop() {
  // put your main code here, to run repeatedly:
   boolean powerKeyValue = false;
   
   adcRawValue = analogRead(VSENS_PIN);
   voltageRaw = adcRawValue * ADC_TO_VOLTAGE;

   if(voltageFiltered < 0){
    voltageFiltered = voltageRaw;
   }
   else{
    voltageFiltered = (0.98 * voltageFiltered) + (0.02 * voltageRaw);
   }

   if(voltageFiltered <= 11.3){
      turnOnTimer = 0;
      powerKeyValue = false;
      goToSleepTimer+=(CYCLE_TIME); 
   }
   else{
    goToSleepTimer = 0;
    if(turnOnTimer >= TURN_ON_DELAY){
      powerKeyValue = true;
    }
    else{
      turnOnTimer += CYCLE_TIME;
    }
   }

   if(0 == detectPowerCycleRequestPulse()){
      turnOnTimer = 0;
      powerKeyValue = false;
   }

  digitalWrite(PWRKEY_PIN, powerKeyValue);
#ifdef DEBUG_EN
  Serial.print("ADC RAW | ");
   Serial.println(adcRawValue);
   Serial.print("Voltage RAW | ");
   Serial.println(voltageRaw);
   Serial.print("Voltage Filtered | ");
   Serial.println(voltageFiltered);
   Serial.print("PwrKey | ");
   Serial.println(((powerKeyValue == false) ? "OFF" : "ON"));
   Serial.println("");
#endif

  if(goToSleepTimer >= 10000){
    goToSleepTimer = 0;

    #ifdef DEBUG_EN
    Serial.println("Going to sleep.");
    delay(2000);
    #endif
    goIdle();
    #ifdef DEBUG_EN
    Serial.println("Woke up");
    #endif
    voltageFiltered = -1;
  }
  else{
     delay(CYCLE_TIME);
  }
  //wdt_reset();

}
