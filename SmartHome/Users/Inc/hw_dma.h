#ifndef HW_DMA
#define HW_DMA

#include "main.h"

#define RX_BUF_SIZE 64

extern uint8_t rx_buf[RX_BUF_SIZE];

void DMA_USART2_RX_Init(void);
void DMA_USART2_TX_Init(void);
#endif