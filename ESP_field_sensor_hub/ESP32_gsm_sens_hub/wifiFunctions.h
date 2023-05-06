const char* ssid = "adsl sandor";
const char* password = "floriflori";

int isWifiAvailable(uint16_t tryConnectTimeOut = 0){
  #define WIFI_TRYCONNECT_CHECK_PERIOD 50
  if(WiFi.status() == WL_CONNECTED){
    return 0;
  }
  if(!tryConnectTimeOut){
    return -1;
  }
  Serial.print(F("WIFI| Connecting to: "));
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  for(int i=0; WiFi.status() != WL_CONNECTED; i++){
    delay(WIFI_TRYCONNECT_CHECK_PERIOD);
    Serial.print(".");
    if(i > (tryConnectTimeOut/WIFI_TRYCONNECT_CHECK_PERIOD)){
      Serial.println(F("\nWIFI| Could not connect-time out"));
      return -1;
    }
  }
  Serial.print(F("\nWIFI| Connected. IP address: "));
  Serial.println(WiFi.localIP());
  return 0;
}
