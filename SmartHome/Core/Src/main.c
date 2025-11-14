/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "hw_gpio.h"
#include "hw_dma.h"
#include "hw_usart.h"
#include "font5x7.h"
#include "st7735.h"
#include "hw_spi.h"
#include "hw_i2c.h"
#include "i2creadwrite.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SHT21_ADDR 0x40
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t tx_hello[] = "Hello\r\n";
uint8_t tx_bye[] = "Bye\r\n";
extern const uint8_t Font5x7[];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void I2C_Scan_LL(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* System interrupt init*/
  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* SysTick_IRQn interrupt configuration */
  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),15, 0));

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  /* USER CODE BEGIN 2 */
	//GPIO_USART2_Init();
	//USART2_Init();
	//DMA_USART2_TX_Init();
	//DMA_USART2_RX_Init();
	SystemClock_Config();
  GPIO_SPI1_Init(); 
  SPI_init();
  GPIO_I2C1_Init();
  I2C_Init();
  LL_I2C_Enable(I2C1);

  ST7735_Init();
  ST7735_FillScreen(COLOR_BLACK);
  ST7735_DrawString(65,60, "Hello!", COLOR_WHITE);
  LL_mDelay(2000);
  ST7735_FillScreen(COLOR_BLACK);
  
  BH1750_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  
  while (1)
  {
    float lux = BH1750_ReadLux();
    char buf[32]; 
    sprintf(buf, "Lux: %.2f   ", lux); 
    ST7735_DrawString(20,45, buf, COLOR_WHITE);
    LL_mDelay(1000);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_0)
  {
  }
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE2);
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {

  }
  LL_Init1msTick(16000000);
  LL_SetSystemCoreClock(16000000);
  LL_RCC_SetTIMPrescaler(LL_RCC_TIM_PRESCALER_TWICE);
}

/* USER CODE BEGIN 4 */


void USART2_IRQHandler(void)
{
	if (LL_USART_IsActiveFlag_IDLE(USART2))
	{
		LL_USART_ClearFlag_IDLE(USART2);
		
		uint8_t received = RX_BUF_SIZE - LL_DMA_GetDataLength(DMA1, LL_DMA_STREAM_5);
		LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_5);
		uint8_t currentData = rx_buf[received-1];
		
		if (currentData == (uint8_t)'0')
		{
			LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_6);
			LL_DMA_SetMemoryAddress(DMA1, LL_DMA_STREAM_6, (uint32_t)tx_bye);
			LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_6, sizeof(tx_bye)-1);
			LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_6);
			while (!LL_DMA_IsActiveFlag_TC6(DMA1)); 
			LL_DMA_ClearFlag_TC6(DMA1);
			while (!LL_USART_IsActiveFlag_TC(USART2));
			LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_6);
		}
		else
		{
			LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_6);
			LL_DMA_SetMemoryAddress(DMA1, LL_DMA_STREAM_6, (uint32_t)tx_hello);
			LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_6, sizeof(tx_hello)-1);
			LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_6);
			while (!LL_DMA_IsActiveFlag_TC6(DMA1)); 
			LL_DMA_ClearFlag_TC6(DMA1);
			while (!LL_USART_IsActiveFlag_TC(USART2));
			LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_6);
		}
		LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_5, RX_BUF_SIZE);
    LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_5);
	}
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
