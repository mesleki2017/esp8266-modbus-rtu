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
#define FIRST_REG 0x0228
#define NUM_REG 2
uint16_t regs[NUM_REG] = {0x0000, 0x0001};

void loop() {
  if (!mb.slave()) {
    //mb.writeHreg(1,40001, 2,cbWrite);
    //mb.writeHreg(1,0x0228, 0x0002, cbWrite);
    //mb.writeHreg(1, 552, (uint16_t*)&0x2, 1, cbWrite);
    mb.writeHreg(SLAVE_ID, FIRST_REG, (uint16_t*)&regs, NUM_REG, cbWrite);
    //mb.readHreg(1, 0x4552, Mread0, 2 , cbWrite);
    //mb.readHreg(1, 552, Mread0, 2 , cbWrite);  //(SlaevID,Address,Buffer,Range of data,Modus call)
    Serial.println("-----------------");
  }
  mb.task();
  delay(1000);
  yield();
}
