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

uint16_t Mread0[60];
int error_code=0;

int cbWrite(Modbus::ResultCode event, uint16_t transactionId, void* data) {
  //Serial.printf_P("Request result: 0x%02X, Mem: %d\n", event, ESP.getFreeHeap());
  // 0x00 = 0,  // Custom. No error
  //0x02 =2 ,   // Output Address not exists
  //0xE4 = 228, // Custom. Operation not finished within reasonable time
  //Serial.println(event);
  error_code=event;
  return true;
};

float InttoFloat(uint16_t Data0,uint16_t Data1) {
  float x;
  unsigned long *p;
  p = (unsigned long*)&x;
  *p = (unsigned long)Data0 << 16 | Data1; //Big-endian
  return(x);
};

void setup() {
  Serial.begin(115200);
  S.begin(38400, SWSERIAL_8N1);
  mb.begin(&S, DE_RE); //Assing Software serial port to Modbus instance for MAX485 chip having DI,DE,RE,RO Pin at TTL side
  mb.master(); //Assing Modbus function as master
 
};

void loop() {
  //memset(Mread0, 0, sizeof(Mread0));// bu kod buffer bosaltma isine yaramadi!!!!
  if (!mb.slave()) {
    
    mb.readHreg(1, 534, Mread0, 8 , cbWrite);  //klemsan pdf (20 Phase 2 Voltage (L-N) float RO
    Serial.print("error_code = ");Serial.println(error_code);
    //Serial.println(InttoFloat(Mread0[0], Mread0[1]));
    Serial.print(Mread0[0]);Serial.print("--");Serial.println(Mread0[1]);
    Serial.print(Mread0[2]);Serial.print("--");Serial.println(Mread0[3]);
    Serial.print(Mread0[4]);Serial.print("--");Serial.println(Mread0[5]);
    Serial.print(Mread0[6]);Serial.print("--");Serial.println(Mread0[7]);
    //Serial.println(InttoFloat(Mread0[0], Mread0[1]));
    
  }
  mb.task();
  delay(1000);
  yield();
  

}
