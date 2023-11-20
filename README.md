# esp8266-modbus-rtu

# klemsandan deger okudum

# faydalandığım linkler
    ## youtube
    - https://www.youtube.com/watch?v=5n0Q8K1_xnU&t=183s
    ## youtube github
    - https://github.com/ashish8284/ESP8266_MODBUS_EM6400NG_SCHNIEDER
    ## library github
    - https://github.com/emelianov/modbus-esp8266



https://github.com/emelianov/modbus-esp8266/blob/master/src/Modbus.h
hata kodalrını anlamak için =
'''
        enum ResultCode {
            EX_SUCCESS              = 0x00, // Custom. No error
            EX_ILLEGAL_FUNCTION     = 0x01, // Function Code not Supported
            EX_ILLEGAL_ADDRESS      = 0x02, // Output Address not exists
            EX_ILLEGAL_VALUE        = 0x03, // Output Value not in Range
            EX_SLAVE_FAILURE        = 0x04, // Slave or Master Device Fails to process request
            EX_ACKNOWLEDGE          = 0x05, // Not used
            EX_SLAVE_DEVICE_BUSY    = 0x06, // Not used
            EX_MEMORY_PARITY_ERROR  = 0x08, // Not used
            EX_PATH_UNAVAILABLE     = 0x0A, // Not used
            EX_DEVICE_FAILED_TO_RESPOND = 0x0B, // Not used
            EX_GENERAL_FAILURE      = 0xE1, // Custom. Unexpected master error
            EX_DATA_MISMACH         = 0xE2, // Custom. Inpud data size mismach
            EX_UNEXPECTED_RESPONSE  = 0xE3, // Custom. Returned result doesn't mach transaction
            EX_TIMEOUT              = 0xE4, // Custom. Operation not finished within reasonable time
            EX_CONNECTION_LOST      = 0xE5, // Custom. Connection with device lost
            EX_CANCEL               = 0xE6, // Custom. Transaction/request canceled
            EX_PASSTHROUGH          = 0xE7, // Custom. Raw callback. Indicate to normal processing on callback exit
            EX_FORCE_PROCESS        = 0xE8  // Custom. Raw callback. Indicate to force processing on callback exit
        };
'''

# write kısmında baya zorlandım ama bu kod işe yaradı
    - https://github.com/emelianov/modbus-esp8266/issues/202
    burdan referans aldım.
alltaki kendi kodum
.somuç olarak bu kod klemsan enerji analizöürünün baudrate ni değiştirdi

'''
    #include <ModbusRTU.h>
    #include <SoftwareSerial.h>
    ModbusRTU mb;
    int DE_RE = 0; //D4  For MAX485 chip
    int RX = 12;
    int TX = 13;
    SoftwareSerial S(RX, TX);//D6/D7  (RX , TX)
    #define SLAVE_ID 0x01
    #define FIRST_REG 0x0228
    #define NUM_REG 2
    uint16_t regs[NUM_REG] = {0x0000, 0x0001};
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
    void loop() {
      if (!mb.slave()) {
            mb.writeHreg(SLAVE_ID, FIRST_REG, (uint16_t*)&regs, NUM_REG, cbWrite);
      }
      mb.task();
      delay(1000);
      yield();
    }
'''

# enerji analizörü röle çıkışını on off yapabildim zaman aralığı içinde
'''
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
    #define NUM_REG 2// kaç register a yazilcak veya okunacak  ornegin 4002 register ından 2 okuncak veya yazilack

    //https://github.com/emelianov/modbus-esp8266/issues/24
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
       delay(20*1000);
       if (!mb.slave()) {
        uint16_t regs22[NUM_REG] = {0,0};//4002 ye role off komutu
        mb.writeHreg(SLAVE_ID, 4002, (uint16_t*)&regs22, NUM_REG, cbWrite);
        while (mb.slave()) {
               mb.task();
               delay(1);
            };
        Serial.println("-----------------");
      };
       delay(20*1000);
       yield();

    }
'''
# https://github.com/emelianov/modbus-esp8266/issues/24
yukarıdaki kodu , bu aşağıdaki koda göre yazdım. peşpeşe komut gönderme konusunda ki problemi çözdü bu kod
Sequental readIsts (readHreg/writeHreg/whatever) unable to run because of ModbusRTU nature. That is request is unable to run until got responce for previuose one. (That's protocol limitation not the library) No requests queue inside library is implemented.
If it suitable to lock execution till responce or timeout you can write:
'''
  while (waitingForACK)
  {

    timer_function();

    if (ackCheckTimeout)
    {
      if (!Modbus_Master.slave())
      {
        Modbus_Master.readIsts(NODE_1_SLAVE_ID, ISTS_ADDRESS, &Node_1_ackStatus, 1, NULL);
        while (Modbus_Master.slave()) {
           Modbus_Master.task();
           delay(1);
        }
        Modbus_Master.readIsts(NODE_2_SLAVE_ID, ISTS_ADDRESS, &Node_2_ackStatus, 1, NULL);
        while (Modbus_Master.slave()) {
           Modbus_Master.task();
           delay(1);
        }
      }

      Serial.println(Node_1_ackStatus);
      Serial.println(Node_2_ackStatus);

      if (!Node_1_ackStatus && !Node_2_ackStatus)
      {
           readyForNextData = true;
           waitingForACK = false;
      }
      ackCheckTimeout = false;
    }

    //Modbus_Master.task();
    yield();
  }
'''


