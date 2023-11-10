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

