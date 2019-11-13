#include "MCP23008.h"

MCP23008::MCP23008()
{
  _Addr = MCP23008_ADDR_0;
}

MCP23008::MCP23008(uint8_t Addr)
{
  _Addr = Addr;
}

// Functions that mimic Arduino digital pins

void MCP23008::pin_mode(uint8_t pin, uint8_t dir)
{
  if(pin > 7) return;
  uint8_t _iodir = ReadRegister(MCP23008_IODIR);
  if(dir == INPUT)
  {
    _iodir |= 1 << pin;
  } else
    {
      _iodir &= ~(1 << pin);
    }
  WriteRegister(MCP23008_IODIR,_iodir);
}

void MCP23008::digital_write(uint8_t pin, uint8_t state)
{
  if(pin > 7) return;
  uint8_t _gpio = read_GPIO();
  if(state == HIGH)
  {
    _gpio |= 1 << pin;
  } else
    {
      _gpio &= ~(1 << pin);
    }
  write_GPIO(_gpio);  
}

bool MCP23008::digital_read(uint8_t pin)
{
  if(pin > 7) 
  {
    return false;
  } else
    {
      return (ReadRegister(MCP23008_GPIO) >> pin) & 0x01;
    }
}

// Functions for manipulating bits

void MCP23008::set_pull_up_bit(uint8_t BitNum, boolean BitValue)
{
  WriteRegisterBit(MCP23008_GPPU,BitNum,BitValue);
}

void MCP23008::set_direction_bit(uint8_t BitNum, boolean BitValue)
{
  WriteRegisterBit(MCP23008_IODIR,BitNum,BitValue);
}

void MCP23008::set_polarity_bit(uint8_t BitNum, boolean BitValue)
{
  WriteRegisterBit(MCP23008_IPOL,BitNum,BitValue);
}

void MCP23008::enable_interrupt_bit(uint8_t BitNum, boolean BitValue)
{
  WriteRegisterBit(MCP23008_GPINTEN,BitNum,BitValue);
}

void MCP23008::set_interrupt_control_bit(uint8_t BitNum, boolean BitValue)
{
  WriteRegisterBit(MCP23008_INTCON,BitNum,BitValue);
}

boolean MCP23008::read_IO_bit(uint8_t BitNum)
{
   if(BitNum > 7) 
  {
    return false;
  } else
    {
      return (read_GPIO() >> BitNum) & 0x01;
    }
}

void MCP23008::write_IO_bit(uint8_t BitNum, boolean BitValue)
{
  if(BitNum > 7) return;
  uint8_t _gpio = read_GPIO();
  if(BitValue == true)
  {
    _gpio |= 1 << BitNum;
  } else
    {
      _gpio &= ~(1 << BitNum);
    }
  write_GPIO(_gpio);
}

// Functions for manipulating entire registers

void MCP23008::set_direction(uint8_t RegVal)
{
  WriteRegister(MCP23008_IODIR, RegVal);
}

void MCP23008::set_pull_up(uint8_t RegVal)
{
  WriteRegister(MCP23008_GPPU, RegVal);
}

void MCP23008::set_polarity(uint8_t RegVal)
{
  WriteRegister(MCP23008_IPOL, RegVal);
}

void MCP23008::enable_interrupt(uint8_t RegVal)
{
  WriteRegister(MCP23008_GPINTEN, RegVal);
}

void MCP23008::set_interrupt_control(uint8_t RegVal)
{
  WriteRegister(MCP23008_INTCON, RegVal);
}

uint8_t MCP23008::read_GPIO()
{
  return ReadRegister(MCP23008_GPIO);
}

void MCP23008::write_GPIO(uint8_t RegVal)
{
  WriteRegister(MCP23008_GPIO, RegVal);
}

// Private Functions 

boolean MCP23008::ReadRegisterBit(uint8_t Reg, uint8_t BitNum){
  if((ReadRegister(Reg) & (0x01 << BitNum)) == 0){
    return false;
  } else {
      return true;
  }
}

void MCP23008::WriteRegisterBit(uint8_t Reg,uint8_t BitNum, boolean BitValue){
  uint8_t _ReadReg,_WriteReg,_WriteVal;
  
  switch(Reg){
    case MCP23008_GPINTEN:    
      _ReadReg = MCP23008_GPINTEN;
      _WriteReg = MCP23008_GPINTEN;
      break;
    case MCP23008_GPIO:    
      _ReadReg = MCP23008_OLAT;
      _WriteReg = MCP23008_GPIO;
      break;
    case MCP23008_GPPU:    
      _ReadReg = MCP23008_GPPU;
      _WriteReg = MCP23008_GPPU;
      break;
    case MCP23008_IODIR:    
      _ReadReg = MCP23008_IODIR;
      _WriteReg = MCP23008_IODIR;
      break;
    case MCP23008_IOCON:    
      _ReadReg = MCP23008_IOCON;
      _WriteReg = MCP23008_IOCON;
      break;    
  }
  
  _WriteVal = ReadRegister(_ReadReg);

  if(BitValue == HIGH){
    _WriteVal |= 1 << BitNum; 
  } else {
      _WriteVal &= ~(1 << BitNum);
    }
  
  WriteRegister(_WriteReg,_WriteVal);
}

uint8_t MCP23008::ReadRegister(uint8_t Reg)
{
  Wire.beginTransmission(_Addr);
  Wire.write(Reg);
  Wire.endTransmission();
  Wire.requestFrom((int)(_Addr),1);
  uint8_t _retValue = Wire.read();
  Wire.endTransmission();
  return _retValue;
}

void MCP23008::WriteRegister(uint8_t Reg, uint8_t Val) 
{
  Wire.beginTransmission(_Addr);
  Wire.write(Reg);
  Wire.write(Val);
  Wire.endTransmission();
}
