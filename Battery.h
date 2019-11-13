#ifndef Battery_H
#define Battery_H

#include "arduino.h"

#define NUMBER_OF_CELLS       40
#define MAF_BUFFER_LEN        3

class Battery 
{
  public:
      Battery();
      void init();
      void updateCellVoltage(unsigned char, float);
     
      float cell_voltage[NUMBER_OF_CELLS];
      float maf_buffer[NUMBER_OF_CELLS][MAF_BUFFER_LEN];
      
  private:

      

};
#endif
