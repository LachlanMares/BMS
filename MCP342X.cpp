#include "MCP342X.h"

MCP342X::MCP342X()
{
  _devAddr = MCP342X_DEFAULT_ADDRESS;
  _currentChannel = MCP342X_CHANNEL_1;
}

MCP342X::MCP342X(unsigned char address) 
{
  _devAddr = address;
}

bool MCP342X::testConnection() 
{
  Wire.beginTransmission(_devAddr);
  return (Wire.endTransmission() == 0);
}

void MCP342X::configure(unsigned char configData) 
{
  _configReg = configData;
  Wire.beginTransmission(_devAddr);
  Wire.write(_configReg);
  Wire.endTransmission();
}

unsigned char MCP342X::getConfigReg(void) 
{
  return _configReg;
}

void MCP342X::setExtPointers(float* ch1,float* ch2,float* ch3,float* ch4)
{
  _ch1 = ch1;
  _ch2 = ch2;
  _ch3 = ch3;
  _ch4 = ch4;
}

bool MCP342X::startConversion(void) 
{
  Wire.beginTransmission(_devAddr);
  Wire.write(_configReg | MCP342X_RDY);
  return (Wire.endTransmission() == 0);
}

bool MCP342X::Update(void) 
{
  bool _retVal = false;
  if(newReadingInt())
  {
    switch (_currentChannel)
    {
      case MCP342X_CHANNEL_1:
        *_ch1 = getReadingFloat();
        _currentChannel = MCP342X_CHANNEL_2;
        startConversion(_currentChannel);
        break;
        
      case MCP342X_CHANNEL_2:
        *_ch2 = getReadingFloat();
        _currentChannel = MCP342X_CHANNEL_3;
        startConversion(_currentChannel);
        break;  

      case MCP342X_CHANNEL_3:
        *_ch3 = getReadingFloat();
        _currentChannel = MCP342X_CHANNEL_4;
        startConversion(_currentChannel);
        break;
        
      case MCP342X_CHANNEL_4:
        *_ch4 = getReadingFloat();
        _currentChannel = MCP342X_CHANNEL_1;
        startConversion(_currentChannel);
        _retVal = true;
        break; 
    }
  }
  return _retVal;
}

bool MCP342X::startConversion(unsigned char channel) 
{
  Wire.beginTransmission(_devAddr);
  _configReg = ((_configReg & ~MCP342X_CHANNEL_MASK) | (channel & MCP342X_CHANNEL_MASK));
  Wire.write(_configReg | MCP342X_RDY);
  return (Wire.endTransmission() == 0);
}

bool MCP342X::newReadingInt()
{
  bool _retVal = false;
  unsigned char _i=0;
  unsigned int _i2cdata[3];
  
  Wire.requestFrom(_devAddr,3);
  while(Wire.available())
  {
    _i2cdata[_i] = Wire.read();
    _i++;
  }
  Wire.endTransmission();
  
  if(((_i2cdata[2] & MCP342X_RDY) == 0x00) && _i == 3)
  {
    _lastReadingInt = (_i2cdata[1] << 8) + _i2cdata[0];
    _retVal = true;
  }
  return _retVal;
}

bool MCP342X::newReadingUint()
{
  bool _retVal = false;
  unsigned char _i=0;
  unsigned long _i2cdata[4];

  Wire.requestFrom(_devAddr,4);
  while(Wire.available())
  {
    _i2cdata[_i] = Wire.read();
    _i++;
  }
  Wire.endTransmission();
  
  if(((_i2cdata[3] & MCP342X_RDY) == 0x00) && _i == 4)
  {
    _lastReadingUint = (_i2cdata[2] << 16) + (_i2cdata[1] << 8) + _i2cdata[0];
    _retVal = true;
  }
  return _retVal;
}

int MCP342X::getReadingInt()
{
  return _lastReadingInt;
}

long MCP342X::getReadingUint()
{
  return _lastReadingUint;
}

float MCP342X::getReadingFloat()
{
  return (float)(_lastReadingInt);
}


