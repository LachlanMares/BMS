#include "CellBalancer.h"
#include <Wire.h>

CellBalancer::CellBalancer(unsigned char number_of_cells, unsigned char enable_pin) : PCA9685() 
{
  _cell_count = number_of_cells;
  _enable_pin = enable_pin;
  _disabled = true;
}

void CellBalancer::init(float pwm_freq)
{
  pinMode(_enable_pin,OUTPUT);
  disable();
  
  // PCA9685 0
  if(_cell_count > 0) 
  {
    initPCA9685Ext(PCA9685_ADDR_0);
    setPwmFreqExt(PCA9685_ADDR_0, pwm_freq);
  }
  
  // PCA9685 1
  if(_cell_count > 16) 
  {
    initPCA9685Ext(PCA9685_ADDR_1);
    setPwmFreqExt(PCA9685_ADDR_1, pwm_freq);
  }
    
  // PCA9685 2
  if(_cell_count > 32) 
  {
    initPCA9685Ext(PCA9685_ADDR_2);
    setPwmFreqExt(PCA9685_ADDR_2, pwm_freq);
  }
  
  // PCA9685 3
  if(_cell_count > 48) 
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
  
  if(_cell_count > 0) { allLedsOffExt(PCA9685_ADDR_0);}
  if(_cell_count > 16) 
  {
    allLedsOffExt(PCA9685_ADDR_1);
  }
    
  if(_cell_count > 32) 
  {
    allLedsOffExt(PCA9685_ADDR_2);
  }
  
  if(_cell_count > 48) 
  {
    allLedsOffExt(PCA9685_ADDR_3);
  }
}

void CellBalancer::setCellDutyCycle(unsigned char cell_number, unsigned char duty_cycle)
{
  if(!_disabled)
  {
    unsigned char _duty_cycle = constrain(duty_cycle, 0, 100);
    unsigned char _cell_number = constrain(cell_number, 0, _cell_count);
  
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

void CellBalancer::updateCellBalancer(float pack_voltage, void* cell_voltages, void* pack)
{

  batteryPackStruct _pack;
  _pack.pack_voltage = pack_voltage;
  _pack.minimum_cell_voltage = 1000.0f;
  _pack.maximum_cell_voltage = 0.0f;
  
  float _cell_voltages[_cell_count];
   
  memcpy(&_cell_voltages, cell_voltages, sizeof(float)*_cell_count);
       
  float _average_cell_voltage = pack_voltage/_cell_count;
  float _pack_voltage_sum = 0.0f;
    
  for(int i=0;i<_cell_count;i++)
  {
    _pack.minimum_cell_voltage = _cell_voltages[i] < _pack.minimum_cell_voltage ? _cell_voltages[i] : _pack.minimum_cell_voltage;
    _pack.maximum_cell_voltage = _cell_voltages[i] > _pack.maximum_cell_voltage ? _cell_voltages[i] : _pack.maximum_cell_voltage;
    _pack_voltage_sum += _cell_voltages[i];
  }
    
  _pack.voltage_error = pack_voltage - _pack_voltage_sum;
    
  memcpy(&pack, &_pack, sizeof(batteryPackStruct));

  if(!_disabled)
  {
        
  }
}


