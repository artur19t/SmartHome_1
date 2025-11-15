#include "i2creadwrite.h"
#include "st7735.h"

#define I2C_TIMEOUT 10000
#define I2C_TIMEOUT_MS 100

uint16_t Read2Bytes(uint8_t address)
{
  uint8_t data[2];

  LL_I2C_GenerateStartCondition(I2C1);
  while(!LL_I2C_IsActiveFlag_SB(I2C1));

  LL_I2C_TransmitData8(I2C1, (address) | 1); // read
  while(!LL_I2C_IsActiveFlag_ADDR(I2C1));
  LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_NACK);
  LL_I2C_EnableBitPOS(I2C1);
  (void)I2C1->SR2;

  while(!LL_I2C_IsActiveFlag_BTF(I2C1));
  LL_I2C_GenerateStopCondition(I2C1);
  
  data[0] = LL_I2C_ReceiveData8(I2C1);
  data[1] = LL_I2C_ReceiveData8(I2C1);
  LL_I2C_DisableBitPOS(I2C1);
  uint16_t raw = (data[0] << 8) | data[1];
  
  return raw;
}

void ReadNBytes(uint8_t address, uint8_t *buf, uint16_t N)
{
  LL_I2C_GenerateStartCondition(I2C1);
  while(!LL_I2C_IsActiveFlag_SB(I2C1));

  LL_I2C_TransmitData8(I2C1, (address) | 1);
  while(!LL_I2C_IsActiveFlag_ADDR(I2C1));
  LL_I2C_DisableBitPOS(I2C1);
  (void)I2C1->SR2;
  uint16_t i = 0;
  for (; i < (N-3); i++)
  {
    LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_ACK);
    while(!LL_I2C_IsActiveFlag_RXNE(I2C1));
    buf[i] = LL_I2C_ReceiveData8(I2C1);
  }
   
  LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_ACK);
  while(!LL_I2C_IsActiveFlag_BTF(I2C1));
  LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_NACK);
  buf[i++] = LL_I2C_ReceiveData8(I2C1);
  
  while(!LL_I2C_IsActiveFlag_BTF(I2C1));
  LL_I2C_GenerateStopCondition(I2C1);
  buf[i++] = LL_I2C_ReceiveData8(I2C1);
  buf[i++] = LL_I2C_ReceiveData8(I2C1);
}

float BH1750_ReadLux(void)
{
  uint8_t data[2];
  uint16_t raw = Read2Bytes(BH1750_ADDR);
  
  return (float)raw / 1.2f;
}

float SHT21_ReadTemperature(void)
{
    I2C_WriteCmd(SHT21_NO_HOLD_TEMP, SHT21_ADDR);  // no hold master

    LL_mDelay(100);     
    uint8_t buf[3] = {0};

    ReadNBytes(SHT21_ADDR, buf, 3);
    buf[1] &= 0b11111100;
    uint16_t raw = (buf[0] << 8) | buf[1];
    float t = -46.85f + (175.72f * raw) / 65536.0f;
    return t;
}

float SHT21_ReadHumidity(void)
{
    I2C_WriteCmd(SHT21_NO_HOLD_HUMID, SHT21_ADDR);  // no hold master

    LL_mDelay(50);        

    uint8_t buf[3] = {0};

    ReadNBytes(SHT21_ADDR, buf, 3);
    buf[1] &= 0b11111100;
    uint16_t raw = (buf[0] << 8) | buf[1];
    float rh = -6.0f + (125.0f * raw) / 65536.0f;
    return rh;
}

void BH1750_Init(void)
{
  if (I2C_WriteCmd(BH1750_CMD_POWER_ON, BH1750_ADDR) != 0) {
    ST7735_DrawString(20,30, "BH1750: POWER_ON failed\n", COLOR_GREEN);
    return;
  }
  LL_mDelay(10);

  if (I2C_WriteCmd(BH1750_CMD_RESET, BH1750_ADDR) != 0) {
    ST7735_DrawString(30,30, "BH1750: RESET failed (ignored)\n", COLOR_GREEN);
    return;
  }
  LL_mDelay(10);

  if (I2C_WriteCmd(BH1750_CMD_CONT_H, BH1750_ADDR) != 0) {      
    ST7735_DrawString(30,30, "BH1750: START MEASUREMENT failed\n", COLOR_GREEN);
    return;
  }
  LL_mDelay(200);
}

static int I2C_WriteCmd(uint8_t cmd, uint8_t address)
{
  uint32_t t;

  LL_I2C_GenerateStartCondition(I2C1);

  t = 200000;
  while (!LL_I2C_IsActiveFlag_SB(I2C1)) 
  { 
    if(--t==0) 
      goto err; 
  }
  LL_I2C_TransmitData8(I2C1, (uint8_t)(address)); 
  
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
