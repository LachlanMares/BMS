#ifndef PCA9685_H
#define PCA9685_H

#include "arduino.h"

#define PCA9685_SUBADR1   0x02
#define PCA9685_SUBADR2   0x03
#define PCA9685_SUBADR3   0x04

#define PCA9685_MODE1     0x00
#define PCA9685_PRESCALE  0xFE

#define FIRST_LED_ADDR    0x06

class PCA9685 
{
  public:
      PCA9685();
      PCA9685(unsigned char);
      void initPCA9685(void);
      void resetPCA9685(void);
      void setPwmFreq(float);
      void setLedPwm(unsigned char, unsigned int, unsigned int);
      void setLedDutyCycle(unsigned char, unsigned int);

      void initPCA9685Ext(unsigned char);
      void resetPCA9685Ext(unsigned char);
      void setPwmFreqExt(unsigned char, float);
      void setLedPwmExt(unsigned char, unsigned char, unsigned int, unsigned int);
      void setLedDutyCycleExt(unsigned char, unsigned char, unsigned int);


  private:
      unsigned char i2c_address;
      unsigned char i2cRead(unsigned char);
      void i2cWrite(unsigned char, unsigned char);

      unsigned char i2cReadExt(unsigned char, unsigned char);
      void i2cWriteExt(unsigned char, unsigned char, unsigned char);
};
#endif
