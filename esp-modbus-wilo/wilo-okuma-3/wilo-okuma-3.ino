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
int my_array_size=sizeof(Mread0)/sizeof(uint16_t);
int error_code=9999;

int cihaz_id=3;
int tarama_no=0;
int myBaud[] = {9600,19200,38400,115200};
Config myPar[] = {SWSERIAL_8N1,SWSERIAL_8N2};

int cbWrite(Modbus::ResultCode event, uint16_t transactionId, void* data) {
  //Serial.printf_P("Request result: 0x%02X, Mem: %d\n", event, ESP.getFreeHeap());
  // 0x00 = 0,  // Custom. No error
  //0x02 =2 ,   // Output Address not exists
  //0xE4 = 228, // Custom. Operation not finished within reasonable time
  Serial.println(event);
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

String text1="";
String data_to_string(String kkk,uint16_t myArray[], int length) {
  text1=kkk;
  for (byte i = 0; i < length; i++) {
    //Serial.print(myArray[i]);Serial.print(",");
    String l1n=String(myArray[i]);
    text1=text1+","+l1n;
  };
  Serial.println(text1);
  Serial.println(" ");
  return text1;
}
void setup() {
  ESP.eraseConfig();
  Serial.begin(115200);
  S.setTimeout(5000);//isime yaramadi;
  S.begin(myBaud[0], myPar[0]);
  Serial.print("myBaud = ");Serial.println(myBaud[0]);
  Serial.print("myParity = ");Serial.println(myPar[0]);
  uint32_t timeout = 100;
  mb.setInterFrameTime(timeout);//isime yaramadi;

  mb.master(); //Assing Modbus function as master
  mb.begin(&S, DE_RE); //Assing Software serial port to Modbus instance for MAX485 chip having DI,DE,RE,RO Pin at TTL side
  delay(2000);
  Serial.print("MODBUSRTU_TIMEOUT = ");Serial.println(MODBUSRTU_TIMEOUT);
 
};
//https://github.com/emelianov/modbus-esp8266/blob/master/keywords.txt
int start_register=500;//https://cms.media.wilo.com/dcidocpfinder/wilo181064/1552306/wilo181064.html

void loop() {
  
   if (!mb.slave()) {
    
    //mb.readHreg(3,start_register, Mread0,my_array_size, cbWrite);// enerji analizorunde sadece holding register okumustum
    mb.readIreg(3,start_register, Mread0,my_array_size, cbWrite);// wilo pompa da is ,nput registerda okudum
    
    while (mb.slave()) {
           mb.task();
           delay(100);
        };
    Serial.println("------------------------------------------------");
    Serial.print("error_code");Serial.println(error_code);
    
     delay(100);
 
    String mytext1=String(data_to_string(String(start_register)+"--",Mread0,my_array_size));
    Serial.println(mytext1);
    

  };
  //start_register=start_register+1;
  delay(1000);
  yield();
  

}
