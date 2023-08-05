#define MAX_SOL_LOG_MESSAGE 30  /*C00147V12125L0001T 2535*/
#define SOL_LOG_DISABLED 0
#define SOL_LOG_DATA_BUFF_LEN 32

SoftwareSerial solarLogSerial(36, 33); /* Serial has to be inverted, because it is connected via inverting opto coupler, only RX is used*/

int16_t sol_voltage;
int16_t sol_voltage_max;
int16_t sol_voltage_min;

int16_t sol_chargeCurrent;
int16_t sol_chargeCurrent_max;
int16_t sol_chargeCurrent_min;

int16_t sol_light;
float sol_batTemperature;

char solLogData[SOL_LOG_DATA_BUFF_LEN];

void printCharArray(){
  for(int i=0; i < SOL_LOG_DATA_BUFF_LEN; i++){
    Serial.print(solLogData[i]);
  }
}
int8_t updateSolLoggerData(){
   if(SOL_LOG_DISABLED){
    return 0;
   }
   if(!solarLogSerial.available()){
      return 0;
   }
   
   uint16_t numOfCharsRead = 0;
   uint16_t i=0; 
   for(i=0; i < SOL_LOG_DATA_BUFF_LEN; i++){
    solLogData[i] = '\0';
   }
   i=0;
   while (solarLogSerial.available() > 0 && numOfCharsRead < MAX_SOL_LOG_MESSAGE) {
    numOfCharsRead++;
    char inChar = solarLogSerial.read();
    /* @TODO: maybe check if chars are ascii alphanumeric*/
    if(i < SOL_LOG_DATA_BUFF_LEN){/*inChar != ' ' &&*/
      solLogData[i] = inChar;
      i++;
      Serial.print(inChar);
    }
  }
 /* Serial.println(solLogData);*/
  if(numOfCharsRead >= MAX_SOL_LOG_MESSAGE){
    Serial.println("SOLLOG | [ WR ] msg too long");
  }

  uint16_t reading = 0;
  for(int i=0; i < SOL_LOG_DATA_BUFF_LEN; i++){
    
    if(solLogData[i] == 'C' && (i + 5) < SOL_LOG_DATA_BUFF_LEN){
          bool alphanumeric = true;
          for(int j=1; j <= 5; j++){
            if((solLogData[i+j] - '0') > 9 || (solLogData[i+j] - '0') < 0){
              alphanumeric = false;
              break;
            }
          }
          if(alphanumeric){
            sol_chargeCurrent = ((solLogData[i+1] - '0') * 10000) + ((solLogData[i+2] - '0') * 1000) + ((solLogData[i+3] - '0') * 100) + ((solLogData[i+4] - '0') * 10) + ((solLogData[i+5] - '0') * 1);
            Serial.print("charge_current: ");
            Serial.println(sol_chargeCurrent);
          }
    }

     if(solLogData[i] == 'V' && (i + 5) < SOL_LOG_DATA_BUFF_LEN){
          bool alphanumeric = true;
          for(int j=1; j <= 5; j++){
            if((solLogData[i+j] - '0') > 9 || (solLogData[i+j] - '0') < 0){
              alphanumeric = false;
              break;
            }
          }
          if(alphanumeric){
            sol_voltage =((solLogData[i+1] - '0') * 10000) + ((solLogData[i+2] - '0') * 1000) + ((solLogData[i+3] - '0') * 100) + ((solLogData[i+4] - '0') * 10) + ((solLogData[i+5] - '0') * 1);
            Serial.print("Voltage: ");
            Serial.println(sol_voltage);
          }
    }

    if(solLogData[i] == 'L' && (i + 4) < SOL_LOG_DATA_BUFF_LEN){
          bool alphanumeric = true;
          for(int j=1; j <= 4; j++){
            if((solLogData[i+j] - '0') > 9 || (solLogData[i+j] - '0') < 0){
              alphanumeric = false;
              break;
            }
          }
          if(alphanumeric){
            sol_light =((solLogData[i+1] - '0') * 1000) + ((solLogData[i+2] - '0') * 100) + ((solLogData[i+3] - '0') * 10) + ((solLogData[i+4] - '0') * 1);
            Serial.print("light: ");
            Serial.println(sol_light);
          }
    }

    if(solLogData[i] == 'T' && (i + 5) < SOL_LOG_DATA_BUFF_LEN){
          bool alphanumeric = true;
          for(int j=1; j <= 5; j++){
            if( (solLogData[i+j] - ' ' != 0) && ((solLogData[i+j] - '0') > 9 || (solLogData[i+j] - '0') < 0)){
              alphanumeric = false;
              break;
            }
          }
          if(alphanumeric){
            sol_batTemperature =((solLogData[i+2] - '0') * 10) + ((solLogData[i+3] - '0') * 1) + ((solLogData[i+4] - '0') * 0.1 ) + ((solLogData[i+5] - '0') * 0.01);
            if(solLogData[i+1] - '-' == 0){
              sol_batTemperature *= -1.0;
            }
            Serial.print("bat_temp: ");
            Serial.println(sol_batTemperature);
          }
    }
  }

  if(sol_voltage > sol_voltage_max){
    sol_voltage_max = sol_voltage;
  }
  if(sol_voltage < sol_voltage_min){
    sol_voltage_min = sol_voltage;
  }

  if(sol_chargeCurrent > sol_chargeCurrent_max){
    sol_chargeCurrent_max = sol_chargeCurrent;
  }
  if(sol_chargeCurrent < sol_chargeCurrent_min){
    sol_chargeCurrent_min = sol_chargeCurrent;
  }
}

void addSolLoggerMqttTags(char(& mqttPayloadBuff)[MQTT_PAYLOAD_BUFF_LEN], char(& tempStrBuffer)[_TEMP_STRLEN]){
  if(sol_voltage > 0 && sol_voltage < 99000){
    snprintf(tempStrBuffer, _TEMP_STRLEN, " \"v_sol\":%d.%d,",(int)(sol_voltage/1000), (((int)sol_voltage)%1000)/10);
    strcat(mqttPayloadBuff, tempStrBuffer);
  }
  if(sol_voltage_max > 0 && sol_voltage_max < 99000){
    snprintf(tempStrBuffer, _TEMP_STRLEN, " \"v_solM\":%d.%d,",(int)(sol_voltage_max/1000), (((int)sol_voltage_max)%1000)/10);
    strcat(mqttPayloadBuff, tempStrBuffer);
  }
  if(sol_voltage_min > 0 && sol_voltage_min < 99000){
    snprintf(tempStrBuffer, _TEMP_STRLEN, " \"v_solm\":%d.%d,",(int)(sol_voltage_min/1000), (((int)sol_voltage_min)%1000)/10);
    strcat(mqttPayloadBuff, tempStrBuffer);
  }
  
  if(sol_chargeCurrent >-1 && sol_chargeCurrent < 20000){
    snprintf(tempStrBuffer, _TEMP_STRLEN, " \"sol_charge_i\":%d,",(int)(sol_chargeCurrent));
    strcat(mqttPayloadBuff, tempStrBuffer);
  }
  if(sol_chargeCurrent_min >-1 && sol_chargeCurrent_min < 20000){
    snprintf(tempStrBuffer, _TEMP_STRLEN, " \"sol_charge_im\":%d,",(int)(sol_chargeCurrent_min));
    strcat(mqttPayloadBuff, tempStrBuffer);
  }
  if(sol_chargeCurrent_max >-1 && sol_chargeCurrent_max < 20000){
    snprintf(tempStrBuffer, _TEMP_STRLEN, " \"sol_charge_iM\":%d,",(int)(sol_chargeCurrent_max));
    strcat(mqttPayloadBuff, tempStrBuffer);
  }
  
  if(sol_light >-1 && sol_chargeCurrent < 9999){
    snprintf(tempStrBuffer, _TEMP_STRLEN, " \"sol_light\":%d,",(int)(sol_light));
    strcat(mqttPayloadBuff, tempStrBuffer);
  }
  if(sol_batTemperature >-99 && sol_chargeCurrent < 99){
    snprintf(tempStrBuffer, _TEMP_STRLEN, " \"sol_bat_temp\":%d.%d,",(int)(sol_batTemperature),(int)(abs(sol_batTemperature)*10)%100);
    strcat(mqttPayloadBuff, tempStrBuffer);
  }
  sol_voltage_max = 0;
  sol_voltage_min = 999999;
  sol_chargeCurrent_max = 0;
  sol_chargeCurrent_min = 999999;
}
