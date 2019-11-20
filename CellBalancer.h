#ifndef CellBalancer_H
#define CellBalancer_H

#include "arduino.h"
#include "PCA9685.h"
#include "Structures.h"

#define PCA9685_MAX           4095
#define PCA9685_ADDR_0        0x40
#define PCA9685_ADDR_1        0x41
#define PCA9685_ADDR_2        0x42
#define PCA9685_ADDR_3        0x43
#define NUMBER_OF_CELLS       40


class CellBalancer: public PCA9685
{
  public:
      CellBalancer(unsigned char);
      void init(float);
      void enable();
      void disable();
      void setCellDutyCycle(unsigned char, unsigned char);
      unsigned char getCellDutyCycle(unsigned char, unsigned char*);
      void updateCellBalancer(float, void*);
      
      unsigned char duty_cycle[NUMBER_OF_CELLS];
  private:
 
      boolean _disabled;
      unsigned char _enable_pin;


};
#endif
