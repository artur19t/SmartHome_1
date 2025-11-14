#include "i2creadwrite.h"
#include "st7735.h"

#define I2C_TIMEOUT 10000
#define I2C_TIMEOUT_MS 100

float BH1750_ReadLux(void)
{
  uint8_t data[2];

  LL_I2C_GenerateStartCondition(I2C1);

  while (!LL_I2C_IsActiveFlag_SB(I2C1));

  LL_I2C_TransmitData8(I2C1, BH1750_ADDR | 0x01); 

  while (!LL_I2C_IsActiveFlag_ADDR(I2C1));
  (void)I2C1->SR2; 

  while (!LL_I2C_IsActiveFlag_RXNE(I2C1));
  data[0] = LL_I2C_ReceiveData8(I2C1);

  while (!LL_I2C_IsActiveFlag_RXNE(I2C1));
  data[1] = LL_I2C_ReceiveData8(I2C1);

  LL_I2C_GenerateStopCondition(I2C1);

  uint16_t raw = (data[0] << 8) | data[1];
  return (float)raw / 1.2f;
}

void BH1750_WriteCmd(uint8_t cmd)
{
  LL_I2C_GenerateStartCondition(I2C1);
  while (!LL_I2C_IsActiveFlag_SB(I2C1));

  LL_I2C_TransmitData8(I2C1, BH1750_ADDR);
  while (!LL_I2C_IsActiveFlag_ADDR(I2C1));
  (void)I2C1->SR2; 

  LL_I2C_TransmitData8(I2C1, cmd);
  while (!LL_I2C_IsActiveFlag_TXE(I2C1));

  LL_I2C_GenerateStopCondition(I2C1);
}

void BH1750_Init(void)
{
  if (I2C_WriteCmd(BH1750_CMD_POWER_ON) != 0) {
      ST7735_DrawString(20,30, "BH1750: POWER_ON failed\n", COLOR_GREEN);
      return;
  }
  LL_mDelay(10);

  if (I2C_WriteCmd(BH1750_CMD_RESET) != 0) {
      /* ?? ???????? — ????????? ????? ?????????? RESET */
      // printf("BH1750: RESET failed (ignored)\n");
  }
  LL_mDelay(10);

  if (I2C_WriteCmd(BH1750_CMD_CONT_H) != 0) {      
      ST7735_DrawString(30,30, "BH1750: START MEASUREMENT failed\n", COLOR_GREEN);
      return;
  }
  LL_mDelay(200);
}

static int I2C_WriteCmd(uint8_t cmd)
{
  uint32_t t;

  LL_I2C_GenerateStartCondition(I2C1);

  t = 200000;
  while (!LL_I2C_IsActiveFlag_SB(I2C1)) 
  { 
    if(--t==0) 
      goto err; 
  }

  LL_I2C_TransmitData8(I2C1, (uint8_t)(BH1750_ADDR | 0x00)); 
  
  t = 200000;
  while (!LL_I2C_IsActiveFlag_ADDR(I2C1)) 
  { 
    if(--t==0) 
      goto err; 
  }
  
  (void)I2C1->SR2; 

  t = 200000;
  while (!LL_I2C_IsActiveFlag_TXE(I2C1)) 
  { 
    if(--t==0) 
      goto err;   
  }

  LL_I2C_TransmitData8(I2C1, cmd);

  t = 200000;
  while (!LL_I2C_IsActiveFlag_BTF(I2C1)) 
  { 
    if(--t==0) 
      goto err; 
  }

  LL_I2C_GenerateStopCondition(I2C1);

  return 0;
err:
  
  LL_I2C_GenerateStopCondition(I2C1);
  return -1;
}
