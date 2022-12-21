/*
 *  Functions for interfacing with a SIM 7000G chip, com uses hardware UART on the ESP32
 *  #include <HardwareSerial.h> is a dependency.
 */
//#include <HardwareSerial.h>
#include <SoftwareSerial.h>

//HardwareSerial GSMModemSerial(2); // use UART2
SoftwareSerial GSMModemSerial;

#define GSMBAUD 9600
#define GSMRX_PIN 16
#define GSMTX_PIN 17

#define GETMILISECS() (uint32_t)(int32_t)(esp_timer_get_time()/1000)  /// efficient? Yes. Cursed? Also yes.
#define GETELAPSEDMS() ((uint32_t)(int32_t)(esp_timer_get_time()/1000) - this->startedMs)

#define IN_BUFF_LEN 300

class SIM7000{
  private:
    char serInBuff[IN_BUFF_LEN];

    uint32_t timeLimitms = 5000;
    uint32_t startedMs = 0;
    unsigned int buffIndex = 0;
    int atCmdRes = -3;      /// 0 -ok, -1 -error, -2 unknown if succeeded, -3 no response

    /*char* mqttPayload;
    unsigned int mqttPayloadLen = 0;*/
    
    
  public:

    SIM7000(){  
       //GSMModemSerial.begin(GSMBAUD, SERIAL_8N1, GSMRX_PIN, GSMTX_PIN);     ///TODO: for some reason this does not get executed? Some C++ quirk probably, so moved to separate function fix me later
    }
    void init(){
       delay(1000);
       GSMModemSerial.begin(9600, SWSERIAL_8N1, GSMRX_PIN, GSMTX_PIN, false);
       //GSMModemSerial.begin(GSMBAUD, SERIAL_8N1, GSMRX_PIN, GSMTX_PIN);
       Serial.println("GSMModemSerial STARTED");
       delay(1000);
    }

    char* getInBuffer(){
      return this->serInBuff;
    }

    /*void setMqttPayload(char* mqttPaylodPtr, unsigned int len){
      this->mqttPayload = mqttPaylodPtr;
      this->mqttPayloadLen = len;
    }*/
    /*char* getMqttPayloadString(){
      //char mqttMsg[] = "faszo";
      char mqttMsgBuffer[200];
      snprintf(mqttMsgBuffer, sizeof(mqttPayload)/sizeof(char), "%s\r", mqttPayload);
      return mqttMsgBuffer;
    }*/

    int readSerial(){
      if(!GSMModemSerial.available()){
        return -1;
      }
      //serInBuff[0] = '\0';
      unsigned int waitCharTime = 0;

      while(GSMModemSerial.available() || (waitCharTime < 100)){
        if(!GSMModemSerial.available()){
          waitCharTime++;
          if(waitCharTime >= 100){
            return 0;
          }
          delay(1);     ///TODO: Blocking delay here!
          continue;
        }
        waitCharTime=0;
        
        serInBuff[buffIndex] = (char)GSMModemSerial.read();
#ifdef _SIMDEBUG_PRINTRAW
        Serial.print((char) serInBuff[buffIndex]);
#endif
        buffIndex++;
        if(!(buffIndex < IN_BUFF_LEN)){
          Serial.println("[Er] in buff full");
          return -1;
        }
      }
      return 0;
      
    }
    int debugPrintResponse(){
      while(GSMModemSerial.available()){
        Serial.print((char)GSMModemSerial.read());
      }
    }

    void dbgPrintGSMBuffer(){
      Serial.println("");
      Serial.print("GSM| ");
      for(int i=0; i<(IN_BUFF_LEN - 1); i++){
        if(serInBuff[i] == '\0'){
          break;
        }
        Serial.print((char)serInBuff[i]);
        if(serInBuff[i] == '\n' && serInBuff[i+1] != '\0'){
          Serial.print("GSM| ");
        }
      }
      //Serial.println(" ");
      return;
    }
    

    int atPrint(const char* atCommand, const char* responseOKKey, uint32_t timeout = 5000, const char* responseFAILKey = "ERROR"){ 

      delay(100);
      timeLimitms = timeout;
      GSMModemSerial.print(atCommand); 
      startedMs = GETMILISECS();
      
#ifdef _SIMDEBUG
      Serial.print("\nSending: ");
      Serial.println(atCommand);
      debugPrintResponse();
#endif
      
      uint8_t res = -2;
      this->buffIndex = 0;
      serInBuff[0] = '\0';
      while( res != 0 && (GETELAPSEDMS() < this->timeLimitms)){
        if(readSerial() != 0){
          delay(10);
          continue;
        }
        serInBuff[buffIndex] = '\0';
        atCmdRes = -2;

        if(responseFAILKey != "___" && strstr(serInBuff, responseFAILKey) != NULL){
          atCmdRes = -1;
          break;
        }
        if(strstr(serInBuff, responseOKKey) != NULL){
          atCmdRes = 0;
          break;
        }
      }
      serInBuff[buffIndex] = '\0';  /// add terminator at the end of str
      dbgPrintGSMBuffer();

//#ifdef _SIMDEBUG
      Serial.print("AT response: ");
      switch(atCmdRes){
        case 0:
          Serial.print("[ OK ]");
        break;
        case -1:
          Serial.print("[ FAIL ]");
        break;
        case -2:
          Serial.print("[ UNKNOWN ]");
        break;
        case -3:
          Serial.print("[ NO RESP. ]");
        break;
      }  
      Serial.print("\nelapsed: ");
      Serial.println(GETELAPSEDMS());
//#endif
     
      return atCmdRes; 
    }
    
};
