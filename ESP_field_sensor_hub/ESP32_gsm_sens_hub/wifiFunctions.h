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
  DebugSerial.print(F("WIFI| Connecting to: "));
  DebugSerial.println(ssid);

  WiFi.begin(ssid, password);

  for(int i=0; WiFi.status() != WL_CONNECTED; i++){
    delay(WIFI_TRYCONNECT_CHECK_PERIOD);
    DebugSerial.print(".");
    if(i > (tryConnectTimeOut/WIFI_TRYCONNECT_CHECK_PERIOD)){
      DebugSerial.println(F("\nWIFI| Could not connect-time out"));
      return -1;
    }
  }
  DebugSerial.print(F("\nWIFI| Connected. IP address: "));
  DebugSerial.println(WiFi.localIP());
  return 0;
}
