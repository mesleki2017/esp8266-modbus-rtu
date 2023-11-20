/*
  Modbus Library for Arduino Example - Modbus RTU Client
  Read Holding Registers from Modbus RTU Server in blocking way
  ESP8266 Example
  
  (c)2020 Alexander Emelianov (a.m.emelianov@gmail.com)
  https://github.com/emelianov/modbus-esp8266
*/


#include <ModbusRTU.h>
#include <SoftwareSerial.h>
ModbusRTU mb;


int DE_RE = 0; //D4  For MAX485 chip
int RX = 12;
int TX = 13;


SoftwareSerial S(RX, TX);//D6/D7  (RX , TX)

uint16_t Mread0[2];



bool cbWrite(Modbus::ResultCode event, uint16_t transactionId, void* data) {
  Serial.printf_P("Request result: 0x%02X, Mem: %d\n", event, ESP.getFreeHeap());
  return true;
}

void setup() {
  Serial.begin(115200);
  S.begin(38400, SWSERIAL_8N1);
  mb.begin(&S, DE_RE); //Assing Software serial port to Modbus instance for MAX485 chip having DI,DE,RE,RO Pin at TTL side
  mb.master(); //Assing Modbus function as master
  Serial.println(); //Print empty line
  Serial.println(sizeof(Mread0)); //Reaing size of first array

}
#define SLAVE_ID 0x01
#define FIRST_REG 4002 
#define NUM_REG 2

//https://github.com/emelianov/modbus-esp8266/issues/24
void loop() {
  if (!mb.slave()) {
    uint16_t regs[NUM_REG] = {0,1111};
    mb.writeHreg(SLAVE_ID, 4000, (uint16_t*)&regs, NUM_REG, cbWrite);
    while (mb.slave()) {
           mb.task();
           delay(1);
        }
    //mb.readHreg(1, 588, Mread0, 2 , cbWrite);  //(SlaevID,Address,Buffer,Range of data,Modus call)
    Serial.println("-----------------");
  }
  //mb.task();
  delay(1000);
    if (!mb.slave()) {
    uint16_t regs22[NUM_REG] = {0,1};
    mb.writeHreg(SLAVE_ID, 4002, (uint16_t*)&regs22, NUM_REG, cbWrite);
    while (mb.slave()) {
           mb.task();
           delay(1);
        }
   // mb.readHreg(1, 588, Mread0, 2 , cbWrite);  //(SlaevID,Address,Buffer,Range of data,Modus call)
    Serial.println("-----------------");
   

  }
   delay(1000);
       if (!mb.slave()) {
    uint16_t regs22[NUM_REG] = {0,0};
    mb.writeHreg(SLAVE_ID, 4002, (uint16_t*)&regs22, NUM_REG, cbWrite);
    while (mb.slave()) {
           mb.task();
           delay(1);
        }
   // mb.readHreg(1, 588, Mread0, 2 , cbWrite);  //(SlaevID,Address,Buffer,Range of data,Modus call)
    Serial.println("-----------------");
   

  }
   delay(1000);
    yield();

}
