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

uint16_t Mread0[30];
int my_array_size=sizeof(Mread0)/sizeof(uint16_t);
int error_code=0;

bool cbWrite(Modbus::ResultCode event, uint16_t transactionId, void* data) {
  //Serial.printf_P("Request result: 0x%02X, Mem: %d\n", event, ESP.getFreeHeap());
  // 0x00 = 0,  // Custom. No error
  //0x02 =2 ,   // Output Address not exists
  //0xE4 = 228, // Custom. Operation not finished within reasonable time
  //Serial.println(event);
  error_code=event;
  return true;
};


bool cbWrite2(Modbus::ResultCode event, uint16_t transactionId, void* data) {
  //Serial.printf_P("Request result: 0x%02X, Mem: %d\n", event, ESP.getFreeHeap());
  // 0x00 = 0,  // Custom. No error
  //0x02 =2 ,   // Output Address not exists
  //0xE4 = 228, // Custom. Operation not finished within reasonable time
  //Serial.println(event);
  error_code=event;
  Serial.println("ashdahda");
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
    Serial.println(my_array_size);
    int start_register=534;
    mb.readHreg(1,start_register, Mread0,my_array_size-2, cbWrite);  //klemsan pdf (20 Phase 2 Voltage (L-N) float RO
    Serial.print("error_code = ");Serial.println(error_code);

    //Serial.println(InttoFloat(Mread0[0], Mread0[1]));
    for (int aaa = 1; aaa < my_array_size; aaa=aaa+2) {
      //Serial.print(Mread0[aaa-1]);Serial.print("--");Serial.println(Mread0[aaa]);
     Serial.print(start_register+aaa-1);Serial.print("--");Serial.print(start_register+aaa); Serial.print(" =");
     Serial.print(InttoFloat(Mread0[aaa-1], Mread0[aaa]));Serial.print("--");
     Serial.print(Mread0[aaa-1]);Serial.print("--");Serial.println(Mread0[aaa]);
      }
    
  }
  mb.task();
  delay(1000);
 
  


  if (!mb.slave()) {
  mb.writeHreg(1,553 ,2 ,cbWrite2);
  while(mb.slave()) {
    mb.task();
    delay(100);
    Serial.println("hhhhhhh");
    }
  delay(2000);
  
  }

 yield();
}
