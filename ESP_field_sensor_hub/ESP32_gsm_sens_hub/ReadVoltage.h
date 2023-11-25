#define VPPIN 36
#define VNPIN 39
#define BOX_TEMP_PIN 35

double vSol = -1;  // Solar - vp
double vBat = -1;  // Battery - vn
double rtuBoxTemp0 = -99;
double rtuBoxTemp1 = -99;

double vSolNow = -1;  // Solar - vp
double vBatNow = -1;  // Battery - vn
double rtuBoxTemp0Now = -99;
double rtuBoxTemp1Now = -99;

#define TOFFS 0
#define ADC_TO_BAT_VOLTAGE(ARG) ((ARG)*((double)12.6/871.0))
#define ADC_TO_SOLAR_VOLTAGE(ARG) ((ARG)*((double)23.66/1647.0))
/*#define ADC_TO_DEGC(ARG) (((double)ARG* (2970.0/1927))/10.0) - 273.15/**/


// function to print a device address
void printAddress(DallasTemperature& sensors, DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

// function to print a device's resolution
void printResolution(DallasTemperature& sensors, DeviceAddress deviceAddress)
{
  Serial.print("Resolution: ");
  Serial.print(sensors.getResolution(deviceAddress));
  Serial.println();
}

void setupDS18B20(DallasTemperature& sensors){
  /** one wire start */
  sensors.begin();
  sensors.getDeviceCount();
  
  if (!sensors.getAddress(hub_temp0, 0)) Serial.println("Unable to find address for DS18B20 0");
  if (!sensors.getAddress(hub_temp1, 1)) Serial.println("Unable to find address for DS18B20 1");

  // show the addresses we found on the bus
  /*Serial.print("Device 0 Address: ");
  printAddress(sensors, hub_temp0);
  Serial.println();

  Serial.print("Device 1 Address: ");
  printAddress(sensors, hub_temp1);
  Serial.println();*/

  // set the resolution to 9 bit per device
  sensors.setResolution(hub_temp0, HUBDS18B20_TEMPERATURE_PRECISION);
  sensors.setResolution(hub_temp1, HUBDS18B20_TEMPERATURE_PRECISION);

  /*Serial.print("Device 0 Resolution: ");
  Serial.print(sensors.getResolution(hub_temp0), DEC);
  Serial.println();

  Serial.print("Device 1 Resolution: ");
  Serial.print(sensors.getResolution(hub_temp1), DEC);
  Serial.println();*/
}

void readDS18B20(DallasTemperature& sensors){
  sensors.requestTemperatures();
  Serial.println("DONE");

  float tempC = sensors.getTempC(hub_temp0);
  if(tempC == DEVICE_DISCONNECTED_C) 
  {
    Serial.println("Error: Could not read temperature data");
    return;
  }

  rtuBoxTemp0 = tempC;
  //Serial.print("hub_temp0 Temp C: ");
  //Serial.print(tempC);

  tempC = sensors.getTempC(hub_temp1);
  if(tempC == DEVICE_DISCONNECTED_C) 
  {
    Serial.println("Error: Could not read temperature data");
    return;
  }
  //Serial.print("hub_temp1 Temp C: ");
  //Serial.print(tempC);
  rtuBoxTemp1 = tempC;
}
void readVoltages(){
  
  vBatNow = ADC_TO_BAT_VOLTAGE(((double)analogRead(VNPIN)));  /// no filtering. let's see how it does
  vSolNow = ADC_TO_SOLAR_VOLTAGE(((double)analogRead(VPPIN)));
  readDS18B20(sensors);

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

  
  if(rtuBoxTemp0Now > -99 && rtuBoxTemp0Now < 99){
    if(rtuBoxTemp0 <= -99){
      rtuBoxTemp0 = rtuBoxTemp0Now;
    }
    else{
      rtuBoxTemp0 = (0.98*rtuBoxTemp0) + (0.02*rtuBoxTemp0Now);
    } 
  }

  if(rtuBoxTemp1Now > -99 && rtuBoxTemp1Now < 99){
    if(rtuBoxTemp1 <= -99){
      rtuBoxTemp1 = rtuBoxTemp1Now;
    }
    else{
      rtuBoxTemp1 = (0.98*rtuBoxTemp1) + (0.02*rtuBoxTemp1Now);
    } 
  }

 #if 1
  Serial.print("VBAT| ");
  Serial.println(vBatNow);
  Serial.print("VSOL| ");
  Serial.println(vSolNow);
  /*Serial.print("TEMP| ");
  Serial.println(rtuBoxTemp);*/
  
  Serial.print("ADC| ");
  Serial.println(analogRead(VNPIN));
  Serial.println(analogRead(VPPIN));
 #endif
}

bool isPowerSavingEnabled(){

  readVoltages();
  if(vBat <= 11.3){
    return true;
  }else{
    return false;
  }
  
}

void addVoltageMqttTags(char(& mqttPayloadBuff)[MQTT_PAYLOAD_BUFF_LEN], char(& tempStrBuffer)[_TEMP_STRLEN]){
  if(vBat < 99.99 && vBat >= 0){
    snprintf(tempStrBuffer, _TEMP_STRLEN, " \"v_bat\":%d.%d,",(int16_t)(vBat), ((int16_t)(vBat*100))%100);
    strcat(mqttPayloadBuff, tempStrBuffer);
  }
  else{
    Serial.println(F("vBat is out of bounds!"));
  }
  if(vSol < 99.99 && vSol >= 0){
    snprintf(tempStrBuffer, _TEMP_STRLEN, " \"v_sol\":%d.%d,",(int16_t)(vSol), ((int16_t)(vSol*100))%100);
    strcat(mqttPayloadBuff, tempStrBuffer);
  }
  else{
    Serial.println(F("vSol is out of bounds!"));
  }
  if(rtuBoxTemp0 < 99.99 && rtuBoxTemp0 >= -99){
    snprintf(tempStrBuffer, _TEMP_STRLEN, " \"box_temp_0\":%d.%d,",(int16_t)(rtuBoxTemp0), ((int16_t)(rtuBoxTemp0*100))%100);
    strcat(mqttPayloadBuff, tempStrBuffer);
  }
  else{
    Serial.println(F("rtuBoxTemp0 is out of bounds!"));
  }

  if(rtuBoxTemp1 < 99.99 && rtuBoxTemp1 >= -99){
    snprintf(tempStrBuffer, _TEMP_STRLEN, " \"box_temp_1\":%d.%d,",(int16_t)(rtuBoxTemp1), ((int16_t)(rtuBoxTemp1*100))%100);
    strcat(mqttPayloadBuff, tempStrBuffer);
  }
  else{
    Serial.println(F("rtuBoxTemp0 is out of bounds!"));
  }
}

/* @TODO: move this to proper place*/
void addESPResetStsMqttTags(char(& mqttPayloadBuff)[MQTT_PAYLOAD_BUFF_LEN], char(& tempStrBuffer)[_TEMP_STRLEN]){

  switch(lastRebootReason){
    case -1:
      snprintf(tempStrBuffer, _TEMP_STRLEN, " \"reset\":\"no_reboot\",");
      break;
    case 1:
      snprintf(tempStrBuffer, _TEMP_STRLEN, " \"reset\":\"power_on\",");
      break;
    case 3:
      snprintf(tempStrBuffer, _TEMP_STRLEN, " \"reset\":\"soft_reset\",");
      break;
    case 6:
      snprintf(tempStrBuffer, _TEMP_STRLEN, " \"reset\":\"wdt_timeout\",");
      break;
    case 7:
      snprintf(tempStrBuffer, _TEMP_STRLEN, " \"reset\":\"brown_out_reset\",");
      break;
    default:
      snprintf(tempStrBuffer, _TEMP_STRLEN, " \"reset\":\"unknown_%d\",",(int)(lastRebootReason));
      break;
  } 
  
  strcat(mqttPayloadBuff, tempStrBuffer);
  
  snprintf(tempStrBuffer, _TEMP_STRLEN, " \"free_heap\":%d,", esp_get_free_heap_size());
  strcat(mqttPayloadBuff, tempStrBuffer);
  
  lastRebootReason = -1;
}
