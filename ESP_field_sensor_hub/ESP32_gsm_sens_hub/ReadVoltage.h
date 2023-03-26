#define VPPIN 36
#define VNPIN 39

double vSol = -1;          // Solar - vp
double vBat = -1;  // Battery - vn

void readVoltages(){
  #define VN_SCALE 12.3/623 /// real voltage / ADC value
  #define VN_WEIGHT 5.0/100
  
  //static double vp = 13.5;
  //static double vn = 13.5;

  if(vBat == -1){
    vBat = VN_SCALE*analogRead(VNPIN);
  }
  else{
    vBat = vBat*(1 -VN_WEIGHT) + (VN_WEIGHT*VN_SCALE*analogRead(VNPIN));
  }
  
  //vp = VP_SCALE*analogRead(VPPIN);        /// TODO: fix this, somehow not measuring right voltage...

  Serial.print("VBAT| ");
  Serial.println(vBat);
  Serial.print("VSOL| ");
  Serial.println(vSol);
  Serial.print("ADC| ");
  Serial.println(analogRead(VNPIN));
  Serial.println(analogRead(VPPIN));
}

void addVoltageMqttTags(char(& mqttPayloadBuff)[500], char(& tempStrBuffer)[_TEMP_STRLEN]){

  #define PWR0 1%10
  #define PWR1 1%100/10
  #define PWR2 1%1000/100
  #define PWR3 1%10000/1000

  snprintf(tempStrBuffer, _TEMP_STRLEN, " \"v_bat\":%d%d.%d%d,",(int)(vBat*100)*PWR3, (int)(vBat*100)*PWR2, (int)(vBat*100)*PWR1, (int)(vBat*100)*PWR0);
  strcat(mqttPayloadBuff, tempStrBuffer);
  snprintf(tempStrBuffer, _TEMP_STRLEN, " \"v_solar\":%d%d.%d%d,",(int)(vSol*100)*PWR3, (int)(vSol*100)*PWR2, (int)(vSol*100)*PWR1, (int)(vSol*100)*PWR0);
  strcat(mqttPayloadBuff, tempStrBuffer);
   
}
