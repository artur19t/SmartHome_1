#include "hw_i2c.h"

void I2C_Init(void)
{
  LL_I2C_InitTypeDef I2C_InitStruct = {0};
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);

  I2C_InitStruct.PeripheralMode  = LL_I2C_MODE_I2C;
  I2C_InitStruct.ClockSpeed      = 100000; // 100kHz
  I2C_InitStruct.DutyCycle       = LL_I2C_DUTYCYCLE_2;
  I2C_InitStruct.OwnAddress1     = 0;
  I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK;
  I2C_InitStruct.OwnAddrSize     = LL_I2C_OWNADDRESS1_7BIT;
  LL_I2C_Init(I2C1, &I2C_InitStruct);
  LL_I2C_Enable(I2C1);
}
