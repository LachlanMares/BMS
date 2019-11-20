#include "CellBalancer.h"
#include <Wire.h>

CellBalancer::CellBalancer(unsigned char enable_pin) : PCA9685() 
{
  _enable_pin = enable_pin;
  _disabled = true;
  _number_of_pcas = ceil(NUMBER_OF_CELLS/16);
}

void CellBalancer::init(float pwm_freq)
{
  pinMode(_enable_pin,OUTPUT);
  disable();
  
  switch(_number_of_pcas)
  {
    case 1:
      initPCA9685Ext(PCA9685_ADDR_0);
      setPwmFreqExt(PCA9685_ADDR_0, pwm_freq);
      break;
    
    case 2:
      initPCA9685Ext(PCA9685_ADDR_0);
      setPwmFreqExt(PCA9685_ADDR_0, pwm_freq);
      initPCA9685Ext(PCA9685_ADDR_1);
      setPwmFreqExt(PCA9685_ADDR_1, pwm_freq);
      break;
    
    case 3:
      initPCA9685Ext(PCA9685_ADDR_0);
      setPwmFreqExt(PCA9685_ADDR_0, pwm_freq);
      initPCA9685Ext(PCA9685_ADDR_1);
      setPwmFreqExt(PCA9685_ADDR_1, pwm_freq);
      initPCA9685Ext(PCA9685_ADDR_2);
      setPwmFreqExt(PCA9685_ADDR_2, pwm_freq);
      break;
    
    case 4:
      initPCA9685Ext(PCA9685_ADDR_0);
      setPwmFreqExt(PCA9685_ADDR_0, pwm_freq);
      initPCA9685Ext(PCA9685_ADDR_1);
      setPwmFreqExt(PCA9685_ADDR_1, pwm_freq);
      initPCA9685Ext(PCA9685_ADDR_2);
      setPwmFreqExt(PCA9685_ADDR_2, pwm_freq);
      initPCA9685Ext(PCA9685_ADDR_3);
      setPwmFreqExt(PCA9685_ADDR_3, pwm_freq);
      break;

    default:
      break;
  }
}

void CellBalancer::enable()
{
  _disabled = false;
  digitalWrite(_enable_pin,_disabled);
  allOff();
}

void CellBalancer::disable()
{
  _disabled = true;
  digitalWrite(_enable_pin,_disabled);
  allOff();
}


void CellBalancer::allOff()
{
  switch(_number_of_pcas)
  {
    case 1:
      allLedsOffExt(PCA9685_ADDR_0);
      break;
    
    case 2:
      allLedsOffExt(PCA9685_ADDR_0);
      allLedsOffExt(PCA9685_ADDR_1);
      break;
    
    case 3:
      allLedsOffExt(PCA9685_ADDR_0);
      allLedsOffExt(PCA9685_ADDR_1);
      allLedsOffExt(PCA9685_ADDR_2);
      break;
    
    case 4:
      allLedsOffExt(PCA9685_ADDR_0);
      allLedsOffExt(PCA9685_ADDR_1);
      allLedsOffExt(PCA9685_ADDR_2);
      allLedsOffExt(PCA9685_ADDR_3);
      break;

    default:
      break;
  }
}

void CellBalancer::setCellDutyCycle(unsigned char cell_number, unsigned char duty_cycle)
{
  if(!_disabled)
  {
    unsigned char _duty_cycle = constrain(duty_cycle, 0, 100);
    unsigned char _cell_number = constrain(cell_number, 0, NUMBER_OF_CELLS);
  
    if(_cell_number < 16) 
    { 
      setLedDutyCycleExt(PCA9685_ADDR_0, _cell_number, _duty_cycle);
    } else if(_cell_number < 32) 
      { 
        setLedDutyCycleExt(PCA9685_ADDR_1, _cell_number-16, _duty_cycle);
      } else if (_cell_number < 48) 
        {
          setLedDutyCycleExt(PCA9685_ADDR_2, _cell_number-32, _duty_cycle);
        }  else if (_cell_number < 64) 
          {
            setLedDutyCycleExt(PCA9685_ADDR_3, _cell_number-48, _duty_cycle);
          }
  } 
}

void CellBalancer::updateCellBalancer(float pack_voltage, void* pack)
{

  batteryPackStruct _pack;
  unsigned int _buffer[16];
  
  memcpy(&_pack, pack, sizeof(batteryPackStruct));
  
  _pack.pack_voltage = pack_voltage;
  _pack.minimum_cell_voltage = 1000.0f;
  _pack.maximum_cell_voltage = 0.0f;
       
  float _average_cell_voltage = _pack.pack_voltage/NUMBER_OF_CELLS;
  float _pack_voltage_sum = 0.0f;
  unsigned int _mapped_duty_cycle = 0;
    
  for(int i=0;i<NUMBER_OF_CELLS;i++)
  {
    _pack.minimum_cell_voltage = _pack.cell_voltages[i] < _pack.minimum_cell_voltage ? _pack.cell_voltages[i] : _pack.minimum_cell_voltage;
    _pack.maximum_cell_voltage = _pack.cell_voltages[i] > _pack.maximum_cell_voltage ? _pack.cell_voltages[i] : _pack.maximum_cell_voltage;
    _pack_voltage_sum += _pack.cell_voltages[i];
    if(!_disabled)
    {   
      _mapped_duty_cycle = (unsigned int)(map(_pack.cell_voltages[i], _pack.maximum_cell_voltage, _pack.minimum_cell_voltage, 0, PCA9685_MAXVALUE));
      _pack.duty_cycles[i] = _mapped_duty_cycle > PCA9685_DEADBAND ? _mapped_duty_cycle : 0;
    } else
      {
        _pack.duty_cycles[i] = 0;
      }
  }

  if(!_disabled)
  {
    switch(_number_of_pcas)
    {
      case 1:
        memcpy(&_buffer, &_pack.cell_voltages[0], sizeof(_buffer));
        writeAllLedsExt(PCA9685_ADDR_0, *_buffer);
        break;
    
      case 2:
        memcpy(&_buffer, &_pack.cell_voltages[0], sizeof(_buffer));
        writeAllLedsExt(PCA9685_ADDR_0, *_buffer);
        memcpy(&_buffer, &_pack.cell_voltages[16], sizeof(_buffer));
        writeAllLedsExt(PCA9685_ADDR_1, *_buffer);
        break;
    
      case 3:
        memcpy(&_buffer, &_pack.cell_voltages[0], sizeof(_buffer));
        writeAllLedsExt(PCA9685_ADDR_0, *_buffer);
        memcpy(&_buffer, &_pack.cell_voltages[16], sizeof(_buffer));
        writeAllLedsExt(PCA9685_ADDR_1, *_buffer);
        memcpy(&_buffer, &_pack.cell_voltages[32], sizeof(_buffer));
        writeAllLedsExt(PCA9685_ADDR_2, *_buffer);
        break;
    
      case 4:
        memcpy(&_buffer, &_pack.cell_voltages[0], sizeof(_buffer));
        writeAllLedsExt(PCA9685_ADDR_0, *_buffer);
        memcpy(&_buffer, &_pack.cell_voltages[16], sizeof(_buffer));
        writeAllLedsExt(PCA9685_ADDR_1, *_buffer);
        memcpy(&_buffer, &_pack.cell_voltages[32], sizeof(_buffer));
        writeAllLedsExt(PCA9685_ADDR_2, *_buffer);
        memcpy(&_buffer, &_pack.cell_voltages[48], sizeof(_buffer));
        writeAllLedsExt(PCA9685_ADDR_3, *_buffer);
        break;

      default:
        break;
    }
  }
    
  _pack.voltage_error = _pack.pack_voltage - _pack_voltage_sum;  
  memcpy(pack, &_pack, sizeof(batteryPackStruct));
}



