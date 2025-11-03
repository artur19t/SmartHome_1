#include "hw_usart.h"

void USART2_Init(void)
{
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
	
	LL_USART_InitTypeDef USART2_InitStruct = {0};
	USART2_InitStruct.BaudRate = 9600;
	USART2_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
	USART2_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
	USART2_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
	USART2_InitStruct.Parity = LL_USART_PARITY_NONE;
	USART2_InitStruct.StopBits = LL_USART_STOPBITS_1;
	USART2_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
	LL_USART_Init(USART2, &USART2_InitStruct);
	
	LL_USART_ConfigAsyncMode(USART2);
	
	LL_USART_Enable(USART2);
	LL_USART_EnableIT_IDLE(USART2);
	NVIC_SetPriority(USART2_IRQn, 0);
  NVIC_EnableIRQ(USART2_IRQn);
}
