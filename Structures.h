#ifndef Structures_h
#define Structures_h

#include <WString.h>

#define NUMBER_OF_CELLS                   40
#define MAX_DUTY_CYCLE                    100
#define DUTY_CYCLE_DEADBAND               10

typedef struct{
  unsigned int duty_cycles[NUMBER_OF_CELLS];
  float cell_voltages[NUMBER_OF_CELLS];
  float pack_voltage;
  float voltage_error;
  float maximum_cell_voltage;
  float minimum_cell_voltage;
} batteryPackStruct;

#endif
