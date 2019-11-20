#include "CellBalancer.h"
#include <Wire.h>

CellBalancer::CellBalancer(unsigned char enable_pin) : PCA9685() 
{
  _enable_pin = enable_pin;
  _disabled = true;
}

void CellBalancer::init(float pwm_freq)
{
  pinMode(_enable_pin,OUTPUT);
  disable();
  
  // PCA9685 0
  if(NUMBER_OF_CELLS > 0) 
  {
    initPCA9685Ext(PCA9685_ADDR_0);
    setPwmFreqExt(PCA9685_ADDR_0, pwm_freq);
  }
  
  // PCA9685 1
  if(NUMBER_OF_CELLS > 16) 
  {
    initPCA9685Ext(PCA9685_ADDR_1);
    setPwmFreqExt(PCA9685_ADDR_1, pwm_freq);
  }
    
  // PCA9685 2
  if(NUMBER_OF_CELLS > 32) 
  {
    initPCA9685Ext(PCA9685_ADDR_2);
    setPwmFreqExt(PCA9685_ADDR_2, pwm_freq);
  }
  
  // PCA9685 3
  if(NUMBER_OF_CELLS > 48) 
  {
    initPCA9685Ext(PCA9685_ADDR_3);
    setPwmFreqExt(PCA9685_ADDR_3, pwm_freq);
  }
}

void CellBalancer::enable()
{
  _disabled = false;
  digitalWrite(_enable_pin,_disabled);
}

void CellBalancer::disable()
{
  _disabled = true;
  digitalWrite(_enable_pin,_disabled);
  
  if(NUMBER_OF_CELLS > 0) 
  { 
    allLedsOffExt(PCA9685_ADDR_0);
  }
  
  if(NUMBER_OF_CELLS > 16) 
  {
    allLedsOffExt(PCA9685_ADDR_1);
  }
    
  if(NUMBER_OF_CELLS > 32) 
  {
    allLedsOffExt(PCA9685_ADDR_2);
  }
  
  if(NUMBER_OF_CELLS > 48) 
  {
    allLedsOffExt(PCA9685_ADDR_3);
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

unsigned char CellBalancer::getCellDutyCycle(unsigned char cell_number, unsigned char* duty_cycles)
{
  return duty_cycles[cell_number];
}

void CellBalancer::updateCellBalancer(float pack_voltage, void* pack)
{

  batteryPackStruct _pack;
  memcpy(&_pack, pack, sizeof(batteryPackStruct));
  
  _pack.pack_voltage = pack_voltage;
  _pack.minimum_cell_voltage = 1000.0f;
  _pack.maximum_cell_voltage = 0.0f;
       
  float _average_cell_voltage = _pack.pack_voltage/NUMBER_OF_CELLS;
  float _pack_voltage_sum = 0.0f;
  float _mapped_duty_cycle = 0.0f;
    
  for(int i=0;i<NUMBER_OF_CELLS;i++)
  {
    _pack.minimum_cell_voltage = _pack.cell_voltages[i] < _pack.minimum_cell_voltage ? _pack.cell_voltages[i] : _pack.minimum_cell_voltage;
    _pack.maximum_cell_voltage = _pack.cell_voltages[i] > _pack.maximum_cell_voltage ? _pack.cell_voltages[i] : _pack.maximum_cell_voltage;
    _pack_voltage_sum += _pack.cell_voltages[i];
    if(!_disabled)
    {   
      _mapped_duty_cycle = map(_pack.cell_voltages[i], _pack.maximum_cell_voltage, _pack.minimum_cell_voltage, 0, MAX_DUTY_CYCLE);
      _pack.duty_cycles[i] = _mapped_duty_cycle > DUTY_CYCLE_DEADBAND ? _mapped_duty_cycle : 0;
    } else
      {
        _pack.duty_cycles[i] = 0;
      }
  }
    
  _pack.voltage_error = _pack.pack_voltage - _pack_voltage_sum;  
  memcpy(pack, &_pack, sizeof(batteryPackStruct));
}


