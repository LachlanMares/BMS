#ifndef MCP23008_h
#define MCP23008_h

#include "Arduino.h"
#include <Wire.h>

// MCP23017 Registers
#define MCP23008_IODIR       0x00
#define MCP23008_IPOL        0x01
#define MCP23008_GPINTEN     0x02
#define MCP23008_DEFVAL      0x03
#define MCP23008_INTCON      0x04
#define MCP23008_IOCON       0x05
#define MCP23008_GPPU        0x06
#define MCP23008_INTF        0x07
#define MCP23008_INTCAP      0x08
#define MCP23008_GPIO        0x09
#define MCP23008_OLAT        0x0A

#define MCP_OUTPUTS          0x00
#define MCP_INPUTS           0xFF
#define MCP_PULLUP_ALL       0xFF
#define MCP_ALL_ON           0xFF
#define MCP_ALL_OFF          0x00

//Address of MCP23008 IO Expander, 8 addresses available
#define MCP23008_ADDR_0      0x20
#define MCP23008_ADDR_1      0x21
#define MCP23008_ADDR_2      0x22
#define MCP23008_ADDR_3      0x23
#define MCP23008_ADDR_4      0x24
#define MCP23008_ADDR_5      0x25
#define MCP23008_ADDR_6      0x26
#define MCP23008_ADDR_7      0x27

class MCP23008
{
public:
        MCP23008();
        MCP23008(uint8_t);
        
        void pin_mode(uint8_t, uint8_t);
        void digital_write(uint8_t, uint8_t);
        bool digital_read(uint8_t);
        
        void set_pull_up_bit(uint8_t, boolean);
        void set_direction_bit(uint8_t, boolean);
        void set_polarity_bit(uint8_t, boolean);
        void enable_interrupt_bit(uint8_t, boolean); 
        void set_interrupt_control_bit(uint8_t, boolean);
        boolean read_IO_bit(uint8_t);
        void write_IO_bit(uint8_t, boolean);      
        
        void set_direction(uint8_t);
        void set_pull_up(uint8_t);     
        void set_polarity(uint8_t);
        void enable_interrupt(uint8_t);   
        void set_interrupt_control(uint8_t);
        uint8_t read_GPIO();
        void write_GPIO(uint8_t);
           
        
private:
        boolean ReadRegisterBit(uint8_t, uint8_t);
        void WriteRegisterBit(uint8_t, uint8_t, boolean);
        uint8_t ReadRegister(uint8_t);     
        void WriteRegister(uint8_t, uint8_t);  
    
        uint8_t _Addr;        
};

#endif
