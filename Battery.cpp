#include "Battery.h"

Battery::Battery() 
{

}

void Battery::init() 
{
  for(int i=0;i<NUMBER_OF_CELLS;i++)
  {
    cell_voltage[i] = 0.0f;
    for(int j=0;j<MAF_BUFFER_LEN;j++)
    {
      maf_buffer[i][j] = 0.0f;
    }
  }
}

void Battery::updateCellVoltage(unsigned char cell_number, float new_voltage)
{
  if(cell_number < NUMBER_OF_CELLS)
  {
    float _new_voltage = new_voltage;
    
    for(int j=MAF_BUFFER_LEN-1; j>0; j--)
    {
      maf_buffer[cell_number][j] = maf_buffer[cell_number][j-1];
      _new_voltage += maf_buffer[cell_number][j];
    }
    maf_buffer[cell_number][0] = new_voltage;
    cell_voltage[cell_number] = _new_voltage/MAF_BUFFER_LEN;
  }
}

