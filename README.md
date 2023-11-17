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
