#include "CellBalancer.h"
#include "Battery.h"
#include "MCP23008.h"
#include "PCA9685.h"
#include "MCP2515.h"
#include "Structures.h"
#include <Wire.h>

#define MAF_BUFFER_LEN        3
#define PWM_FREQ_SET          500.0f

#define PCA9685_ADDR_0        0x40
#define PCA9685_ADDR_1        0x41
#define PCA9685_ADDR_2        0x42

#define NUMBER_OF_CELLS       40
#define MCP_OUTPUTS           0x00
#define MCP_INPUTS            0xFF
#define MCP_PULLUP_ALL        0xFF
#define MCP_ALL_ON            0xFF
#define MCP_ALL_OFF           0x00

// CAN
#define CAN_SLAVE_SELECT      8
#define CAN_SPEED             CAN_250KBPS

// Cell Balancer
#define BALANCER_ENABLE_PIN   9

Battery battery;
CellBalancer balancer(NUMBER_OF_CELLS, BALANCER_ENABLE_PIN); 
MCP23008 mcp23008;
MCP2515 canbus;

float* cell_voltages;

void setup() 
{
  Wire.begin(); 
  TWBR = ((16000000L/400000L)-16)/2; 
  balancer.init(PWM_FREQ_SET);
  canbus.Init(CAN_SPEED,CAN_SLAVE_SELECT);  
  
  battery.getAllCellVoltages(&cell_voltages);
  mcp23008.set_direction(MCP_OUTPUTS);
  mcp23008.write_GPIO(MCP_ALL_OFF);
}

void loop() {
  // put your main code here, to run repeatedly:

}
