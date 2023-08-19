#define VPPIN 36
#define VNPIN 39
#define BOX_TEMP_PIN 35

double vSol = -1;  // Solar - vp
double vBat = -1;  // Battery - vn
double rtuBoxTemp = -99;

double vSolNow = -1;  // Solar - vp
double vBatNow = -1;  // Battery - vn
double rtuBoxTempNow = -99;

#define TOFFS 0
#define ADC_TO_BAT_VOLTAGE(ARG) (ARG *(12.33/628))
#define ADC_TO_SOLAR_VOLTAGE(ARG) (ARG *(23.66/1647))
#define ADC_TO_DEGC(ARG) (((double)ARG* (2970.0/1927))/10.0) - 273.15/**/

void readVoltages(){
  
  vBatNow = ADC_TO_BAT_VOLTAGE(analogRead(VNPIN));  /// no filtering. let's see how it does
  vSolNow = ADC_TO_SOLAR_VOLTAGE(analogRead(VPPIN));
  rtuBoxTempNow = ADC_TO_DEGC(analogRead(BOX_TEMP_PIN));

  if(vBatNow >= 8 && vBatNow < 50){
    if(vBat <= -1){
      vBat = vBatNow;
    }
    else{
      vBat = (0.98*vBat) + (0.02*vBatNow);
    }
  }

  if(vSolNow >= 0 && vSolNow < 50){
    if( vSol <= -1){
       vSol =  vSolNow;
    }
    else{
      vSol = (0.98*vSol) + (0.02*vSolNow);
    }
    
  }

  if(rtuBoxTempNow > -99 && rtuBoxTempNow < 99){
    if(rtuBoxTemp <= -99){
      rtuBoxTemp = rtuBoxTempNow;
    }
    else{
      rtuBoxTemp = (0.98*rtuBoxTemp) + (0.02*rtuBoxTempNow);
    } 
  }
  
  Serial.print("VBAT| ");
  Serial.println(vBat);
  Serial.print("VSOL| ");
  Serial.println(vSol);
  Serial.print("TEMP| ");
  Serial.println(rtuBoxTemp);
  
  Serial.print("ADC| ");
  Serial.println(analogRead(VNPIN));
  Serial.println(analogRead(VPPIN));
  Serial.println(analogRead(BOX_TEMP_PIN));
}

bool isPowerSavingEnabled(){
  if(vBat > 11.3){
    return false;
  } 
  return true;
}

void addVoltageMqttTags(char(& mqttPayloadBuff)[MQTT_PAYLOAD_BUFF_LEN], char(& tempStrBuffer)[_TEMP_STRLEN]){
  if(vBat < 99.99 && vBat >= 0){
    snprintf(tempStrBuffer, _TEMP_STRLEN, " \"v_bat\":%d.%d,",(int)(vBat), ((int)(vBat*100))%100);
    strcat(mqttPayloadBuff, tempStrBuffer);
  }
  else{
    Serial.println(F("vBat is out of bounds!"));
  }
  if(vSol < 99.99 && vSol >= 0){
    snprintf(tempStrBuffer, _TEMP_STRLEN, " \"v_sol\":%d.%d,",(int)(vSol), ((int)(vSol*100))%100);
    strcat(mqttPayloadBuff, tempStrBuffer);
  }
  else{
    Serial.println(F("vSol is out of bounds!"));
  }
  if(rtuBoxTemp < 99.99 && rtuBoxTemp >= 0){
    snprintf(tempStrBuffer, _TEMP_STRLEN, " \"sol_bat_temp\":%d.%d,",(int)(rtuBoxTemp), ((int)(rtuBoxTemp*100))%100);
    strcat(mqttPayloadBuff, tempStrBuffer);
  }
  else{
    Serial.println(F("rtuBoxTemp is out of bounds!"));
  }
}
