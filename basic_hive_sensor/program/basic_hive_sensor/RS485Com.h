#include "Arduino.h"
#include <SoftwareSerial.h>


/// TODO: split this into settings.h
#ifndef USE_SETTINGS_HEADER
#define BAUD 9600

/// Addr can't be 0 because translates to '\0' and f*cks up all kinds of string functions!!!
#define THIS_DEV_ADDR 66
#define MASTER_DEV_ADDR 70
#define DEV_TYPE_CHAR 'A'

//#define RO_PIN 10   /// RS485 module pins
//#define DI_PIN 11
//#define DE_PIN 12

//#define DEV_IS_MASTER   /// compile with as master (base), or compile as slave (sensor)
#endif

SoftwareSerial serial485(RO_PIN, DI_PIN); // RX, TX

class RS485Com {

#define STARTSYMB '<'         /// Msg starts with 3 STARTSYMB-s
#define ENDSYMB '>'           /// Msg ends with 3 ENDSYMB-s
#define TMOUT 50 //5ms        /// timeout if no next char in TMOUT
#define MSGBUFF_LEN 80       /// length of msg buffer
#define STARTSTOP_BYTESNUM 3  /// 3 is minimum, because 2 could be the same as CRC16 bytes
#define POLL_ALL "POL*"
#define POLL_ALL_LEN (sizeof(POLL_ALL)/ sizeof(char) - 1)

  private:
    char msgBuff[MSGBUFF_LEN];
    bool msgRcvd = false;
    unsigned int strIndex = 0;
    unsigned int strHead = 0;
    unsigned short serCharTimeout = 0;
    //char newChar = ' ';
    int i,j;

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
    /**
     * The same CRC16 function, but for data in ring buffer
     */
    unsigned short crc16Ring(unsigned int head, unsigned int tail) {
      int  crc = 0;
      char i;
      //crc = 0;
      for(; head != tail; head = (head + 1) % MSGBUFF_LEN) {
        crc = crc ^ (int)msgBuff[head] << 8;
        i = 8;
        for(int i=0; i<8; i++){
          if (crc & 0x8000){              /// if msb is set, shift and flip
            crc = crc << 1 ^ 0x1021;
          }
          else{
            crc = crc << 1;               /// if msb is not set, shift only
          }
        }
      }
      return crc;
    }

    void printBuff(){
      for(i=0; i<MSGBUFF_LEN; i++){
        Serial.print(msgBuff[i]);
      }
    }

    void shiftBuffRight(byte places, char shiftInChar){  /// rightmost bit will be lost, but I don't care
      for(; places > 0; places--){
        for(i= (MSGBUFF_LEN - 2); i>=0; i--){
        msgBuff[i+1] = msgBuff[i];
        }
        msgBuff[0] = shiftInChar;
      }
    }
    
  public:
    RS485Com() {
      pinMode(DE_PIN, OUTPUT);
      digitalWrite(DE_PIN, LOW); /// Driver disabled - listening only
      serial485.begin(BAUD);
    }
    /**
    * method checks if there is incoming data from Software serial UART, and reads data into msgBuff ring buffer
    * if the messege contains both start and stop symbols - '<<<' and '>>>', it is valid, otherwise it is thrown out
    * if the transmission stops without '>>>' end symbol, loop waits for next character and eventually times out
    * when a valid msg was recieved, first byte is compared with this->microcontroller's address
    * if the packet is addressed to this device, the CRC16 checksum is checked, if checksum valid:
    * string is recieved and ready for parsing, located in msgBuff between strHead and strIndex - 
    *
    * return 0; - str recieved, and ready for parsing in buffer
    * return 1; - no new msg from UART
    * return -1; - msg has no start or stop symbols/ msg too long/ CRC incorrect/ address incorrect
    */
    int checkInbuf() {
      if (!serial485.available()) {
        return 1; /// no incoming msg
      }
      msgRcvd = false;
      serCharTimeout = 0;
      while (!msgRcvd && (serCharTimeout <= TMOUT)) {
          if (!serial485.available()) {   /// if there is no new character, wait for a bit
            serCharTimeout++;
            delayMicroseconds(10000);
            continue;
          }
          serCharTimeout = 0;
          msgBuff[strIndex] = serial485.read();
          //Serial.println(msgBuff[strIndex]);  /// DBG print incoming character
          if(msgBuff[strIndex] == msgBuff[(strIndex + MSGBUFF_LEN - 1) % MSGBUFF_LEN] && msgBuff[strIndex] == msgBuff[(strIndex + MSGBUFF_LEN - 2) % MSGBUFF_LEN]){ /// check if there are 3 same chars next to each other
            Serial.print(F("3 same chars next to each other at index: "));
            Serial.println(strIndex);
            if(msgBuff[strIndex] == STARTSYMB){
              //Serial.println("Start symbol");
              strHead = strIndex;
            }
            else if(msgBuff[strIndex] == ENDSYMB){
              Serial.println(F("End symbol"));
              msgRcvd = true;  /// will break loop, but first increment strIndex
              strHead = (strHead + 1)%MSGBUFF_LEN;  /// offset msg to skip '<' startchar
              strIndex = (strIndex + MSGBUFF_LEN - 2)%MSGBUFF_LEN;
              //strIndex = (strIndex + 1) % MSGBUFF_LEN;
              break;
            }
          }
          /*if(strIndex == ((strHead + MSGBUFF_LEN - 1)% MSGBUFF_LEN)){ /// TODO: test this some more
            /// ring buffer is full, TLDR
              Serial.println(F("Msg too long:"));
              while(serial485.available()){
                Serial.print((char)serial485.read());
              }
               Serial.println(F("-> dropped characters."));
              return -1;
          }*/
          strIndex = (strIndex + 1) % MSGBUFF_LEN;
          
      }
      if(!msgRcvd){
        Serial.println(F("RS485| [ Er ] Incomplete msg or timeout!"));
        return -1;
      }
      if(msgBuff[strHead] != THIS_DEV_ADDR){  /// check if msg starts with this devices address
        Serial.println(F("RS485| MSG not for me!"));
        return -1;
      }
      //Serial.println(crc16(strPtr, 9));
      //Serial.println(crc16Ring(strHead,strIndex));
      if (crc16Ring(strHead,strIndex) != 0){
        Serial.println(F("RS485| CRC incorrect"));
        return -1;
      }
      //Serial.println("CRC correct");
      strIndex = (strIndex + MSGBUFF_LEN - 2)%MSGBUFF_LEN;  /// drop CRC16 bytes from msg
      Serial.print(F("RS485| Got: "));
      for(i = strHead; i != strIndex; i = (i + 1)%MSGBUFF_LEN){
        Serial.print(msgBuff[i]);
      }
      Serial.println();
      return 0; /// msg read and ready to parse
    }

    int chkIfPoll(){
      for(i = strHead; i != (strIndex + MSGBUFF_LEN - POLL_ALL_LEN + 1)%MSGBUFF_LEN; i = (i + 1)%MSGBUFF_LEN){
        /*Serial.print(msgBuff[i]);
        Serial.println(POLL_ALL_LEN);*/
        j =0;
        for(j=0; j< (POLL_ALL_LEN); j++){
          if(msgBuff[(i + j)%MSGBUFF_LEN] != POLL_ALL[j]){
            break;
          }    
        }
        if (j == (POLL_ALL_LEN)){
           // Serial.println("Found poll req!");
            return 0;
        }
      }
      return -1;
    }
    
#ifndef DEV_IS_MASTER
    void respond2Poll(){
      snprintf(msgBuff, MSGBUFF_LEN, "%d,%d,%d,%d,%d,%d,%d,%d", dht0Temp, dht0Humi, dht1Temp, dht1Humi, dht2Temp, dht2Humi, tiltSensors, pIRsensors);
      this-> sendMsg(MASTER_DEV_ADDR, THIS_DEV_ADDR, DEV_TYPE_CHAR);
    }
#endif

#ifdef DEV_IS_MASTER
    void pollSensor(const char &destinationAddr, const char &sourceAddr){
      snprintf(msgBuff, MSGBUFF_LEN, POLL_ALL);
      this-> sendMsg(destinationAddr, sourceAddr, DEV_TYPE_CHAR);
    }
#endif
/*
 * takes cstring from msgBuff, and adds source, destination address, than calculates 2 bit CRC, adds it to the end of the string, and adds <<< and >>> start and stop bits
 * 
 * Example:
 * <<<AF,POL*⸮⸮>>>
 * <<<FAH,210,370,-999,-999,-999,-999,0,0⸮⸮>>>
 * 
 * 
 */
    int sendMsg(const char &destinationAddr,const char &sourceAddr, const char &devType) {
      uint16_t payloadLen = strlen(msgBuff);
      if(payloadLen < 1){
        Serial.println("msgBuff is empty. Not sending.");
        return;
      }

      shiftBuffRight(4, '>');
      msgBuff[0] = destinationAddr;
      msgBuff[1] = sourceAddr;
      msgBuff[2] = devType;
      msgBuff[3] = ',';
      payloadLen = strlen(msgBuff);
      uint16_t crc = crc16(msgBuff, payloadLen);

      msgBuff[payloadLen+5] = '\0';
      msgBuff[payloadLen+4] = '>';
      msgBuff[payloadLen+3] = '>';
      msgBuff[payloadLen+2] = '>';
      msgBuff[payloadLen+1] = (char)(crc & 0xFF);
      msgBuff[payloadLen] = (char)(crc >> 8);
      shiftBuffRight(3, '<');


      digitalWrite(DE_PIN, HIGH); /// Driver enabled - send
      delay(5);
      //serial485.print(msgBuff);
      serial485.write(msgBuff, (payloadLen + 8));  /// TODO: this should fix '\0' char shenanigans but test it.
      serial485.flush();  /// wait for TXC0, serial buff sent
      Serial.println(msgBuff);
      delay(5);
      digitalWrite(DE_PIN, LOW); /// Driver disabled - listening only

      digitalWrite(13, HIGH);
      delay(100);
      digitalWrite(13, LOW);


#ifdef VERB_SER
      Serial.print("sent: ");
      Serial.println(msgBuff);
#endif

    }
};
