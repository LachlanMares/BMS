#include "CellBalancer.h"
#include <Wire.h>

CellBalancer::CellBalancer(unsigned char number_of_cells) : PCA9685() 
{
  _cell_count = number_of_cells;
}

void CellBalancer::init(float pwm_freq)
{
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
}

void CellBalancer::setCellDutyCycle(unsigned char cell_number, unsigned char duty_cycle)
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
      }
}

unsigned char CellBalancer::getCellDutyCycle(unsigned char cell_number)
{
  unsigned char _duty_cycle;
  unsigned char _cell_number = constrain(cell_number, 0, _cell_count);
 
  return _duty_cycle;
}




