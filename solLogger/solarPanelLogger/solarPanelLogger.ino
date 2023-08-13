#include <SoftwareSerial.h>

#define CURRENT_PIN A0
#define VOLTAGE_PIN A1
#define LIGHT_PIN A2
#define TEMP_PIN A3

#define SEND_PERIOD 5000
#define CYCLE_TIME 200
#define POWER_ON_DELAY 60000/CYCLE_TIME

bool poweronCompleted = false;
uint16_t current;
uint8_t currentDir;
uint16_t voltage;
double light;
double temp;

uint8_t loopCount = 0;

#define TOFFS 0
#define ADC_TO_CURRENT(ARG) (ARG * (95/15))
#define ADC_TO_VOLTAGE(ARG) (ARG *(12350/297))
#define ADC_TO_LIGHT(ARG) (ARG)
#define ADC_TO_DEGC(ARG) ((((double)ARG* (3200/616))/10) - 273.15 + (TOFFS))*100 /**/


char outMsg[100];

SoftwareSerial dataOutSer(8, 9);

char getSign(int val) {
  if (val < 0) {
    return '-';
  }
  return ' ';
}
char getDigit(int val, int digit, bool useSpace = false) {
  if (useSpace && ('0' + (abs(val) % ((int)pow(10, (digit + 1)))) / ((int)pow(10, digit))) == '0') {
    if (val / (int)pow(10, digit + 1) == 0) {
      return ' ';
    }
  }
  return '0' + (abs(val) % ((int)pow(10, (digit + 1)))) / ((int)pow(10, digit));
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(4800);
  Serial.println("DBGSer. OK");

  dataOutSer.begin(4800);

}

void powerOnDelay(){
  if(poweronCompleted){
    return;
  }
  static uint16_t poweronCounter = 0;

  if (((poweronCounter*CYCLE_TIME) % 1000) == 0) {
        Serial.print(".");
      } else if (((poweronCounter*CYCLE_TIME) % 10000) == 0) {
        Serial.println(".");
      }

  poweronCounter++;
  if(poweronCounter >= POWER_ON_DELAY){
    poweronCompleted = true;
  }
}
void loop() {

    powerOnDelay();
  current = (0.98 * current) + (0.02 * ADC_TO_CURRENT(analogRead(CURRENT_PIN)));
  uint16_t newVoltage = ADC_TO_VOLTAGE(analogRead(VOLTAGE_PIN));
  Serial.println(newVoltage);
  if(newVoltage > 8000){
    voltage = (0.98 * voltage) + (0.02 * newVoltage);
  }
  light = ADC_TO_LIGHT(analogRead(LIGHT_PIN));
  temp = (temp * 0.98) + (0.02 * ADC_TO_DEGC(analogRead(TEMP_PIN)));

  delay(CYCLE_TIME);
  loopCount++;
  if (poweronCompleted && (loopCount > SEND_PERIOD / CYCLE_TIME)) {

    snprintf(outMsg, 100, "C%c%c%c%c%cV%c%c%c%c%cL%c%c%c%cT%c%c%c%c%c\0", getDigit(current, 4), getDigit(current, 3), getDigit(current, 2), getDigit(current, 1), getDigit(current, 0), getDigit(voltage, 4), getDigit(voltage, 3), getDigit(voltage, 2), getDigit(voltage, 1), getDigit(voltage, 0), getDigit(light, 3), getDigit(light, 2), getDigit(light, 1), getDigit(light, 0), getSign(temp), getDigit((int16_t)temp, 3), getDigit((int16_t)temp, 2), getDigit((int16_t)temp, 1), getDigit((int16_t)temp, 0)); /*A%c%c%c%cB%c%c%c%c%cC%c%c%c%cD%c%C%c%c*/
    Serial.println(outMsg);
    dataOutSer.println(outMsg);
    loopCount = 0;

    Serial.print("current: ");
    Serial.print(current);
    Serial.print(" ADC: ");
    Serial.println(analogRead(CURRENT_PIN));
    
    Serial.print("volatge: ");
    Serial.print(voltage);
    Serial.print(" ADC: ");
    Serial.println(analogRead(VOLTAGE_PIN));

    Serial.print("light: ");
    Serial.print(light);
    Serial.print(" ADC: ");
    Serial.println(analogRead(LIGHT_PIN));
    
    Serial.print("temp: ");
    Serial.print(temp);
    Serial.print(" ADC: ");
    Serial.println(analogRead(TEMP_PIN));
    Serial.print("\n\n\n");
  }
}
