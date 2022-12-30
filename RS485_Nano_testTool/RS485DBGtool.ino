#include <SoftwareSerial.h>
//#include <SerialTransfer.h>
#include "DHT.h"

#include "dht.h"

#define RO_PIN 10
#define DI_PIN 11
#define DE_PIN 12



DHTSensor dht1(9, DHT22);

double dht0Temp = -99;
double dht0Hum = -99;

SoftwareSerial Serial485 (RO_PIN, DI_PIN); // RX, TX

class RS485COM {

#define BAUD 9600
#define MASTER_ADDR '0'
#define MY_ADDR '0'
#define START_BYTE "<<<"
#define STOP_BYTE ">>>"
#define IN_BUF_SIZE 32

#define POLL_ALL "POL*"

#define CRC_ENABLED 1
#define PRINT2_DBG_SER
#define VERB_SER 1


  private:
  public:

    RS485COM() {
      Serial485.begin(BAUD);
      pinMode(DE_PIN, OUTPUT);
      digitalWrite(DE_PIN, LOW); /// Driver disabled - listening only
    }

    int sendStr(String &inStr) {
      uint16_t crc = crc16(inStr.c_str(), inStr.length());
      inStr += ((char*) &crc)[1];
      inStr += ((char*) &crc)[0];
      inStr = START_BYTE + inStr + STOP_BYTE;

      digitalWrite(13, HIGH);
      digitalWrite(DE_PIN, HIGH); /// Driver enabled - send
      delayMicroseconds(5000);
      Serial485.print(inStr);
      delayMicroseconds(5000);
      digitalWrite(DE_PIN, LOW); /// Driver disabled - listening only
      delay(100);
      digitalWrite(13, LOW);


#ifdef VERB_SER
      Serial.print("sent: ");
      Serial.println(inStr);
#endif

    }
    int parseIncoming(String &rawStr) {

#ifdef VERB_SER
      Serial.println(rawStr);
      Serial.println(rawStr[0]);
#endif

      if (rawStr[0] != MY_ADDR) {
        Serial.print(F("MSG not for me"));
        Serial.println(rawStr[1]);
        return -1;
      }
#ifdef CRC_ENABLED
      if (crc16(rawStr.c_str(), rawStr.length()) != 0) { /// CRC check, zero validation
        Serial.println("[ER] wrong CRC");
        return -1;
      }
#endif

      Serial.println("[OK] got:");

      /// Drop the CRC and the to Addr
      rawStr = rawStr.substring(rawStr.lastIndexOf(","), rawStr.length() - 2);
      Serial.println(rawStr);
      this->respPollAll();
    }
    
    int checkIncoming() {   /// 0 - polled, -1 - error, 1 - nothing
      /// <2,POL*>


      bool msgStarted = false;
      bool rdyToParse = false;
      String inStr = "";

      while (Serial485.available()) {
        char inChar = Serial485.read();
        Serial.print(inChar);
       /* if (inChar == START_BYTE) {
          msgStarted = true;
        }
        else if (inChar == STOP_BYTE) {
          rdyToParse = true;
          break;
        }
        else if (msgStarted) {
          if (inStr.length() >= IN_BUF_SIZE) {
            Serial.println(F("[ER] incoming MSG too long."));
            return -1;
          }
          else {
            inStr.concat(inChar);
          }
        }*/
      }
     /* if (!msgStarted) {
        return 0;
      }
      if (!rdyToParse && msgStarted) {
        Serial.println(F("[ER] incomplete MSG"));
        return -1;
      }
      this->parseIncoming(inStr);*/
      
#ifdef UART_CMDS_ENABLED
      while (Serial.available()) {
        char inChar = Serial.read();

        if (inChar == START_BYTE) {
          msgStarted = true;
        }
        else if (inChar == STOP_BYTE) {
          rdyToParse = true;
          break;
        }
        else if (msgStarted) {
          if (inStr.length() >= IN_BUF_SIZE) {
            Serial.println(F("[ER] incoming MSG too long."));
            return -1;
          }
          else {
            inStr.concat(inChar);
          }
        }
      }
      if (!msgStarted) {
        return 0;
      }
      if (!rdyToParse && msgStarted) {
        Serial.println(F("[ER] incomplete MSG"));
        return -1;
      }
      this->parseIncoming(inStr);
#endif;
      return 0;
    }


    /// from: https://stackoverflow.com/questions/17196743/crc-ccitt-implementation sarvankumar_t
    /// crc16 implementation that works with zero verification CRC-CCITT in XMODEM
    unsigned short crc16(char *ptr, int count) {
      int  crc = 0;
      char i;
      //crc = 0;
      while (--count >= 0) {
        crc = crc ^ (int) * ptr++ << 8;
        i = 8;
        do
        {
          if (crc & 0x8000)
            crc = crc << 1 ^ 0x1021;
          else
            crc = crc << 1;
        } while (--i);
      }
      return crc;
    }

    void respPollAll() {
      String proba = "0,2,A,20.20,80.30,40.20,60.2,50.2,30.28";
      this->sendStr(proba);
    }
};


RS485COM serCom;
void setup() {
  Serial.begin(9600);

  pinMode(13, OUTPUT);

  //serCom.sendQuerry(2);
  String msg = "AF,POL*";
  serCom.sendStr(msg);
  //msg = "2,POL*";
  //serCom.sendStr(msg);

}
void loop() {

  /* serCom.send("hello world!", 0,1);
    delay(500);*/

  serCom.checkIncoming();


  delay(500);
  //dht1.read();
  //dht1.dbgPrint();

}
