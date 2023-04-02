#define VPPIN 36
#define VNPIN 39

double vSol = -1;          // Solar - vp
double vBat = -1;  // Battery - vn

void readVoltages(){
  #define VN_SCALE 12.3/623 /// real voltage / ADC value
  #define VN_WEIGHT 5.0/100

  #define VP_SCALE 12.3/623 /// real voltage / ADC value
  #define VP_WEIGHT 5.0/100
  
  //static double vp = 13.5;
  //static double vn = 13.5;

 /* if(vBat == -1){
    vBat = VN_SCALE*analogRead(VNPIN);
  }
  else{
    vBat = vBat*(1 -VN_WEIGHT) + (VN_WEIGHT*VN_SCALE*analogRead(VNPIN));
  }*/
  
  //vp = VP_SCALE*analogRead(VPPIN);        /// TODO: fix this, somehow not measuring right voltage...


  vBat = VN_SCALE*analogRead(VNPIN);  /// no filtering. let's see how it does
  vSol = VP_SCALE*analogRead(VPPIN);
  
  Serial.print("VBAT| ");
  Serial.println(vBat);
  Serial.print("VSOL| ");
  Serial.println(vSol);
  Serial.print("ADC| ");
  Serial.println(analogRead(VNPIN));
  Serial.println(analogRead(VPPIN));
}

void addVoltageMqttTags(char(& mqttPayloadBuff)[500], char(& tempStrBuffer)[_TEMP_STRLEN]){
  if(vBat < 99.99 && vBat >= 0){
    snprintf(tempStrBuffer, _TEMP_STRLEN, " \"v_bat\":%d.%d,",(int)(vBat), ((int)(vBat*100))%100);
    strcat(mqttPayloadBuff, tempStrBuffer);
  }
  else{
    Serial.println(F("vBat is out of bounds!"));
  }
  if(vSol < 99.99 && vSol >= 0){
    snprintf(tempStrBuffer, _TEMP_STRLEN, " \"v_solar\":%d.%d,",(int)(vSol), ((int)(vSol*100))%100);
    strcat(mqttPayloadBuff, tempStrBuffer);
  }
  else{
    Serial.println(F("vSol is out of bounds!"));
  }
}
