/*
  Modbus Library for Arduino Example - Modbus RTU Client
  Read Holding Registers from Modbus RTU Server in blocking way
  ESP8266 Example
  
  (c)2020 Alexander Emelianov (a.m.emelianov@gmail.com)
  https://github.com/emelianov/modbus-esp8266
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ModbusRTU.h>
#include <SoftwareSerial.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



ModbusRTU mb;


int DE_RE = 0; //D4  For MAX485 chip
int RX = 12;
int TX = 13;


SoftwareSerial S(RX, TX);//D6/D7  (RX , TX)

uint16_t Mread0[2];
int my_array_size=sizeof(Mread0)/sizeof(uint16_t);
int error_code=0;


bool cbWrite(Modbus::ResultCode event, uint16_t transactionId, void* data) {
  Serial.printf_P("Request result: 0x%02X, Mem: %d\n", event, ESP.getFreeHeap());
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
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  display.clearDisplay();
  display.display();
  S.begin(38400, SWSERIAL_8N1);
  mb.begin(&S, DE_RE); //Assing Software serial port to Modbus instance for MAX485 chip having DI,DE,RE,RO Pin at TTL side
  mb.master(); //Assing Modbus function as master
  Serial.println(); //Print empty line
  Serial.println(sizeof(Mread0)); //Reaing size of first array

}
#define SLAVE_ID 0x01
#define NUM_REG 2// kaç register a yazilcak veya okunacak  ornegin 4002 register ından 2 okuncak veya yazilack

//https://github.com/emelianov/modbus-esp8266/issues/24
//display.println(F("------------"));
void loop() {
  if (!mb.slave()) {
    uint16_t regs[NUM_REG] = {0,1111};//roleler enable
    mb.writeHreg(SLAVE_ID, 4000, (uint16_t*)&regs, NUM_REG, cbWrite);
    while (mb.slave()) {
           mb.task();
           delay(1);
        }
    Serial.println("-----------------");
  }
  //mb.task();
  delay(1000);
  if (!mb.slave()) {
    uint16_t regs22[NUM_REG] = {0,1};//4002 ye role on komutu
    mb.writeHreg(SLAVE_ID, 4002, (uint16_t*)&regs22, NUM_REG, cbWrite);
    while (mb.slave()) {
           mb.task();
           delay(1);
        }
    Serial.println("-----------------");
  };
   delay(1000);
   if (!mb.slave()) {
    uint16_t regs22[NUM_REG] = {0,0};//4002 ye role off komutu
    mb.writeHreg(SLAVE_ID, 4002, (uint16_t*)&regs22, NUM_REG, cbWrite);
    while (mb.slave()) {
           mb.task();
           delay(1);
        };
    Serial.println("-----------------");
  };
   delay(1000);
      if (!mb.slave()) {
    uint16_t regs22[NUM_REG] = {0,0};//4002 ye role off komutu
    mb.readHreg(1,20, Mread0,my_array_size, cbWrite);
    while (mb.slave()) {
           mb.task();
           delay(100);
        };
    Serial.println(InttoFloat(Mread0[0], Mread0[1]));
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("V2L-N = ");display.println(InttoFloat(Mread0[0], Mread0[1]));
    display.display();      // Show initial text
    Serial.println("-----------------");
  };
   delay(1000);
   yield();

}
