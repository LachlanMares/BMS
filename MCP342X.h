#ifndef MCP342X_H
#define MCP342X_H

#include "Arduino.h"
#include <Wire.h>

// I2C Address of device
#define MCP342X_DEFAULT_ADDRESS             0x68

// Conversion mode definitions
#define MCP342X_MODE_ONESHOT                0x00
#define MCP342X_MODE_CONTINUOUS             0x10

// Channel definitions
#define MCP342X_CHANNEL_1                   0x00
#define MCP342X_CHANNEL_2                   0x20
#define MCP342X_CHANNEL_3                   0x40
#define MCP342X_CHANNEL_4                   0x60
#define MCP342X_CHANNEL_MASK                0x60

// Sample size definitions - these also affect the sampling rate
#define MCP342X_SIZE_12BIT                  0x00
#define MCP342X_SIZE_14BIT                  0x04
#define MCP342X_SIZE_16BIT                  0x08
#define MCP342X_SIZE_18BIT                  0x0C
#define MCP342X_SIZE_MASK                   0x0C

// Programmable Gain definitions
#define MCP342X_GAIN_1X                     0x00
#define MCP342X_GAIN_2X                     0x01
#define MCP342X_GAIN_4X                     0x02
#define MCP342X_GAIN_8X                     0x03
#define MCP342X_GAIN_MASK                   0x03

// RDY bit definition
#define MCP342X_RDY 0x80

class MCP342X 
{
  public:
          MCP342X();
          MCP342X(unsigned char address);
                  
          bool testConnection(void);
          void configure(uint8_t config);
          unsigned char getConfigReg(void);
          void setExtPointers(float*,float*,float*,float*);
          bool startConversion(void);
          bool startConversion(unsigned char channel);
          bool Update();
          bool newReadingInt();
          bool newReadingUint();
          int getReadingInt();
          long getReadingUint();
          float getReadingFloat();

  private:
          int _devAddr;
          unsigned char _configReg,_currentChannel;
          int _lastReadingInt;
          long _lastReadingUint;
          float* _ch1;
          float* _ch2;
          float* _ch3;
          float* _ch4;
};

#endif /* _MCP342X_H_ */
