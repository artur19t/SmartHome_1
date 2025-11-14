#include "hw_gpio.h"
//PA: 2,3,4,5,7
//PB: 0,1,6,7
void GPIO_USART2_Init(void)
{
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
	
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = LL_GPIO_PIN_3;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}


void GPIO_SPI1_Init(void)
{
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
	
	 
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  // --- SCK ---
  GPIO_InitStruct.Pin = LL_GPIO_PIN_5;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // --- MOSI ---
  GPIO_InitStruct.Pin = LL_GPIO_PIN_7;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // --- CS (Chip Select) ---
  GPIO_InitStruct.Pin = LL_GPIO_PIN_4;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_4); 
	
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_0, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_1, LL_GPIO_MODE_OUTPUT);
	
	GPIO_InitStruct.Pin = LL_GPIO_PIN_0;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

// --- RST (Reset) ---
  GPIO_InitStruct.Pin = LL_GPIO_PIN_1;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void GPIO_I2C1_Init(void)
{
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
  
  LL_GPIO_InitTypeDef I2C1_GPIO_Init = {0};
  //--- SCL ---
  I2C1_GPIO_Init.Alternate = LL_GPIO_AF_4;
  I2C1_GPIO_Init.Mode = LL_GPIO_MODE_ALTERNATE;
  I2C1_GPIO_Init.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  I2C1_GPIO_Init.Pin = LL_GPIO_PIN_6;
  I2C1_GPIO_Init.Pull = LL_GPIO_PULL_NO;
  I2C1_GPIO_Init.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  LL_GPIO_Init(GPIOB, &I2C1_GPIO_Init);
  //--- SDA ---
  I2C1_GPIO_Init.Pin = LL_GPIO_PIN_7;
  LL_GPIO_Init(GPIOB, &I2C1_GPIO_Init);
  
}