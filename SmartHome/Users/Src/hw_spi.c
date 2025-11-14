#include "hw_spi.h"

void SPI_init(void)
{
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);
	
	LL_SPI_InitTypeDef SPI_InitStruct = {0};

  SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;  
  SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;               
  SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;       
  SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;     
  SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;         
  SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;                   
  SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV4; 
  SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;             
  SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE; 
  SPI_InitStruct.CRCPoly = 10;                            

  LL_SPI_Init(SPI1, &SPI_InitStruct);
  LL_SPI_SetStandard(SPI1, LL_SPI_PROTOCOL_MOTOROLA);     
  LL_SPI_Enable(SPI1);
}
