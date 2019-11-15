#include "PCA9685.h"
#include <Wire.h>

PCA9685::PCA9685() 
{
  i2c_address = 0x40;
}

PCA9685::PCA9685(unsigned char addr) 
{
  i2c_address = addr;
}

void PCA9685::initPCA9685() 
{
  i2cWrite(PCA9685_MODE1,0x00);
}

void PCA9685::initPCA9685Ext(unsigned char i2c_addr) 
{
  i2cWriteExt(i2c_addr, PCA9685_MODE1, 0x00);
}

void PCA9685::resetPCA9685(void) 
{
  i2cWrite(PCA9685_MODE1,0x00);
}

void PCA9685::resetPCA9685Ext(unsigned char i2c_addr) 
{
  i2cWriteExt(i2c_addr, PCA9685_MODE1, 0x00);
}

void PCA9685::setPwmFreq(float freq) 
{
  float _prescale_val = ((25000000/4096)/(0.9*freq))-1;
  unsigned char _pre_scale = floor(_prescale_val+0.5);
  unsigned char _old_mode = i2cRead(PCA9685_MODE1);
  unsigned char _new_mode = (_old_mode&0x7F)|0x10; 
  i2cWrite(PCA9685_MODE1, _new_mode);
  i2cWrite(PCA9685_PRESCALE, _pre_scale);
  i2cWrite(PCA9685_MODE1, _old_mode);
  delay(5);
  i2cWrite(PCA9685_MODE1, _old_mode|0xA1);
}

void PCA9685::setPwmFreqExt(unsigned char i2c_addr, float freq) 
{
  float _prescale_val = ((25000000/4096)/(0.9*freq))-1;
  unsigned char _pre_scale = floor(_prescale_val+0.5);
  unsigned char _old_mode = i2cReadExt(i2c_addr, PCA9685_MODE1);
  unsigned char _new_mode = (_old_mode&0x7F)|0x10; 
  i2cWriteExt(i2c_addr, PCA9685_MODE1, _new_mode);
  i2cWriteExt(i2c_addr, PCA9685_PRESCALE, _pre_scale);
  i2cWriteExt(i2c_addr, PCA9685_MODE1, _old_mode);
  delay(5);
  i2cWriteExt(i2c_addr, PCA9685_MODE1, _old_mode|0xA1);
}

void PCA9685::setLedPwm(unsigned char led_number, unsigned int on_time, unsigned int off_time) 
{
  Wire.beginTransmission(i2c_address);
  Wire.write(FIRST_LED_ADDR + 4*led_number);
  Wire.write(on_time);
  Wire.write(on_time>>8);
  Wire.write(off_time);
  Wire.write(off_time>>8);
  Wire.endTransmission();
}

void PCA9685::setLedPwmExt(unsigned char i2c_addr, unsigned char led_number, unsigned int on_time, unsigned int off_time) 
{
  Wire.beginTransmission(i2c_addr);
  Wire.write(FIRST_LED_ADDR + 4*led_number);
  Wire.write(on_time);
  Wire.write(on_time>>8);
  Wire.write(off_time);
  Wire.write(off_time>>8);
  Wire.endTransmission();
}

void PCA9685::setLedDutyCycle(unsigned char led_number, unsigned int duty_cycle)
{
  unsigned char _duty_cycle = constrain(duty_cycle, 0, 100);
  unsigned int _on_time = map(_duty_cycle, 0, 100, 0, 4095);
  unsigned int _off_time = 4095 - _on_time;
  Wire.beginTransmission(i2c_address);
  Wire.write(FIRST_LED_ADDR + 4*led_number);
  Wire.write(_on_time);
  Wire.write(_on_time>>8);
  Wire.write(_off_time);
  Wire.write(_off_time>>8);
  Wire.endTransmission();
}

void PCA9685::setLedDutyCycleExt(unsigned char i2c_addr, unsigned char led_number, unsigned int duty_cycle)
{
  unsigned char _duty_cycle = constrain(duty_cycle, 0, 100);
  unsigned int _on_time = map(_duty_cycle, 0, 100, 0, 4095);
  unsigned int _off_time = 4095 - _on_time;
  Wire.beginTransmission(i2c_addr);
  Wire.write(FIRST_LED_ADDR + 4*led_number);
  Wire.write(_on_time);
  Wire.write(_on_time>>8);
  Wire.write(_off_time);
  Wire.write(_off_time>>8);
  Wire.endTransmission();
}

void PCA9685::allLedsOff()
{
  unsigned int _value_on = 0;
  unsigned int _value_off = 4096;

  Wire.beginTransmission(i2c_address);
  Wire.write(ALLLED_ON_L);
  Wire.write(_value_on);
  Wire.write(_value_on>>8);
  Wire.write(_value_off);
  Wire.write(_value_off>>8);
  Wire.endTransmission();
}

void PCA9685::allLedsOffExt(unsigned char i2c_addr)
{
  unsigned int _value_on = 0;
  unsigned int _value_off = 4096;

  Wire.beginTransmission(i2c_addr);
  Wire.write(ALLLED_ON_L);
  Wire.write(_value_on);
  Wire.write(_value_on>>8);
  Wire.write(_value_off);
  Wire.write(_value_off>>8);
  Wire.endTransmission();
}

void PCA9685::allLedsOn()
{
  unsigned int _value_on = 4096;
  unsigned int _value_off = 0;

  Wire.beginTransmission(i2c_address);
  Wire.write(ALLLED_ON_L);
  Wire.write(_value_on);
  Wire.write(_value_on>>8);
  Wire.write(_value_off);
  Wire.write(_value_off>>8);
  Wire.endTransmission();
}

void PCA9685::allLedsOnExt(unsigned char i2c_addr)
{
  unsigned int _value_on = 4096;
  unsigned int _value_off = 0;

  Wire.beginTransmission(i2c_addr);
  Wire.write(ALLLED_ON_L);
  Wire.write(_value_on);
  Wire.write(_value_on>>8);
  Wire.write(_value_off);
  Wire.write(_value_off>>8);
  Wire.endTransmission();
}

unsigned char PCA9685::i2cRead(unsigned char register_address) 
{
  Wire.beginTransmission(i2c_address);
  Wire.write(register_address);
  Wire.endTransmission();
  Wire.requestFrom((unsigned char)i2c_address, (unsigned char)1);
  return Wire.read();
}

unsigned char PCA9685::i2cReadExt(unsigned char i2c_addr, unsigned char register_address) 
{
  Wire.beginTransmission(i2c_addr);
  Wire.write(register_address);
  Wire.endTransmission();
  Wire.requestFrom((unsigned char)i2c_address, (unsigned char)1);
  return Wire.read();
}

void PCA9685::i2cWrite(unsigned char register_address, unsigned char data) 
{
  Wire.beginTransmission(i2c_address);
  Wire.write(register_address);
  Wire.write(data);
  Wire.endTransmission();
}

void PCA9685::i2cWriteExt(unsigned char i2c_addr, unsigned char register_address, unsigned char data) 
{
  Wire.beginTransmission(i2c_addr);
  Wire.write(register_address);
  Wire.write(data);
  Wire.endTransmission();
}

