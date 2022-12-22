#define VPPIN 36

double vp;
double vn;

void readVoltages(){
  #define VP_SCALE 12.37/1145  /// real voltage / ADC value
  #define VP_WEIGHT 5.0/100
  
  //static double vp = 13.5;
  //static double vn = 13.5;

  
  //vp = vp*(1 -VP_WEIGHT) + (VP_WEIGHT*VP_SCALE*analogRead(VPPIN));
  vp = VP_SCALE*analogRead(VPPIN);                                          /// TODO: fix this, somehow not measuring right voltage...

  Serial.print("VBAT| ");
  Serial.println(vp);
  Serial.print("VSOL| ");
  Serial.println(vn);
  Serial.print("ADC_BAT| ");
  Serial.println(analogRead(VPPIN));
}

void addVoltageMqttTags(char(& mqttPayloadBuff)[500], char(& tempStrBuffer)[_TEMP_STRLEN]){

  #define PWR0 1%10
  #define PWR1 1%100/10
  #define PWR2 1%1000/100
  #define PWR3 1%10000/1000

  snprintf(tempStrBuffer, _TEMP_STRLEN, " \"v_bat\":%d%d.%d%d,",(int)(vp*100)*PWR3, (int)(vp*100)*PWR2, (int)(vp*100)*PWR1, (int)(vp*100)*PWR0);
  strcat(mqttPayloadBuff, tempStrBuffer);
  snprintf(tempStrBuffer, _TEMP_STRLEN, " \"v_solar\":%d%d.%d%d,",(int)(vn*100)*PWR3, (int)(vn*100)*PWR2, (int)(vn*100)*PWR1, (int)(vn*100)*PWR0);
  strcat(mqttPayloadBuff, tempStrBuffer);
   
}
