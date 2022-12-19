#include "Arduino.h"
#include <SoftwareSerial.h>



#define BAUD 9600
//#define MASTER_ADDR '0'
//#define MY_ADDR '2'

/// Addr can't be 0 because translates to '\0' and f*cks up all kinds of string functions!!!
#define THIS_DEV_ADDR 65
#define MASTER_DEV_ADDR 70
#define DEV_TYPE 72

//#define MASTER_ADDR_STR "0"
//#define MY_ADDR_STR "2"
//#define DEV_TYPE_STR "A"


#define START_STR "<"
#define STOP_STR ">"
#define START_BYTE '<'
#define STOP_BYTE '>'

#define POLL_ALL "POL*"

#define CRC_ENABLED 1
#define PRINT2_DBG_SER
#define VERB_SER 1

#define RO_PIN 10
#define DI_PIN 11
#define DE_PIN 12

SoftwareSerial serial485(RO_PIN, DI_PIN); // RX, TX

class RS485Com {

#define STARTSYMB '<'         /// Msg starts with 3 STARTSYMB-s
#define ENDSYMB '>'           /// Msg ends with 3 ENDSYMB-s
#define TMOUT 50 //5ms        /// timeout if no next char in TMOUT
#define MSGBUFF_LEN 50       /// length of msg buffer
#define STARTSTOP_BYTESNUM 3  /// 3 is minimum, because 2 could be the same as CRC16 bytes

  private:
    char msgBuff[MSGBUFF_LEN];
    /// chk in buff******
    bool msgRcvd = false;
    unsigned int buffIndex = 0;
    unsigned int strHead = 0;
    //unsigned int strTail = 0;
    unsigned short serCharTimeout = 0;
    char newChar = ' ';
    ///*******************
  public:
    RS485Com() {
      pinMode(DE_PIN, OUTPUT);
      digitalWrite(DE_PIN, LOW); /// Driver disabled - listening only
      serial485.begin(BAUD);
    }

    int checkInbuf() {
#define LISTEN_VERB
      if (!serial485.available()) {
        return 1; /// no incoming msg
      }
      msgRcvd = false;
      serCharTimeout = 0;
      while (!msgRcvd && (serCharTimeout <= TMOUT)) {
          if (!serial485.available()) {   /// if there is no new character, wait for a bit
            serCharTimeout++;
            delayMicroseconds(100);
            continue;
          }
          serCharTimeout = 0;

          msgBuff[buffIndex] = serial485.read();

          Serial.println(msgBuff[buffIndex]);

          /// check if there are 3 same chars next to each other
          if(msgBuff[buffIndex] == msgBuff[(buffIndex + MSGBUFF_LEN - 1) % MSGBUFF_LEN] && msgBuff[buffIndex] == msgBuff[(buffIndex + MSGBUFF_LEN - 2) % MSGBUFF_LEN]){
            Serial.print("3 same chars next to each other at index: ");
            Serial.println(buffIndex);
            if(msgBuff[buffIndex] == STARTSYMB){
              Serial.println("Start symbol");
              strHead = buffIndex;
            }
            else if(msgBuff[buffIndex] == ENDSYMB){
              Serial.println("End symbol");
              msgRcvd = true;  /// will break loop, but first increment buffIndex
              //buffIndex = (buffIndex + 1) % MSGBUFF_LEN;
              break;
            }
          }
          
          buffIndex = (buffIndex + 1) % MSGBUFF_LEN;
      }
      if(!msgRcvd){
        Serial.println(F("Incomplete msg or timeout!"));
        return -1;
      }
      if(msgBuff[(strHead + 1)%MSGBUFF_LEN] != THIS_DEV_ADDR){  /// check if msg starts with this devices address
        Serial.println(F("MSG not for me!"));
        return -1;
      }

      char* strPtr = &msgBuff[(strHead + 1)%MSGBUFF_LEN] ;

      //Serial.println(crc16(strPtr, 9));
      Serial.println(crc16Ring((strHead + 1)%MSGBUFF_LEN,(buffIndex + MSGBUFF_LEN - 2)%MSGBUFF_LEN));
      if (crc16Ring((strHead + 1)%MSGBUFF_LEN,(buffIndex + MSGBUFF_LEN - 2)%MSGBUFF_LEN) != 0){
        Serial.println("CRC incorrect");
        return -1;
      }
      Serial.println("CRC correct");

      Serial.println(F("Got:"));
      for(int i = (strHead + 1)%MSGBUFF_LEN; i != (buffIndex + MSGBUFF_LEN - 2)%MSGBUFF_LEN; i = (i + 1)%MSGBUFF_LEN){
        Serial.print(msgBuff[i]);
      }
      Serial.println();
      
      return 0; /// msg read and ready to parse
    }

    void clearBuff() {
      for (int i = 0; i < MSGBUFF_LEN; i++) {
        msgBuff[i] = '\0';
      }
    }
    void parseBuff() {
      /*Serial.print("printing msg buff");
      for (int i = 0; i < MSGBUFF_LEN; i++) {
        if (msgBuff[i] == '\0') {
          break;
        }
        Serial.print(msgBuff[i]);
      }
      Serial.println();*/
      Serial.println("Parsing...");
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

    unsigned short crc16Ring(unsigned int head, unsigned int tail) {
      int  crc = 0;
      char i;
      //crc = 0;
      for(; head != tail; head = (head + 1) % MSGBUFF_LEN) {
        crc = crc ^ (int)msgBuff[head] << 8;
        i = 8;
        /*for(int i=0; i<8; i++){
        }*/
        do
        {
          if (crc & 0x8000){              /// if msb is set, shift and flip
            crc = crc << 1 ^ 0x1021;
          }
          else{
            crc = crc << 1;               /// if msb is not set, shift only
          }
        } while (--i);
      }
      return crc;
    }

};
