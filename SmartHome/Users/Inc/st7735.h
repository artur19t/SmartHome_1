#ifndef __ST7735_H
#define __ST7735_H

#include "main.h"
#include <stdint.h>

#define ST7735_WIDTH   160
#define ST7735_HEIGHT  110

#define ST7735_CS_GPIO_Port   GPIOA
#define ST7735_CS_Pin         LL_GPIO_PIN_4

#define ST7735_DC_GPIO_Port   GPIOB
#define ST7735_DC_Pin         LL_GPIO_PIN_0

#define ST7735_RST_GPIO_Port  GPIOB
#define ST7735_RST_Pin        LL_GPIO_PIN_1

//  RGB565
#define COLOR_BLACK       0x0000
#define COLOR_BLUE        0x001F
#define COLOR_RED         0xF800
#define COLOR_GREEN       0x07E0
#define COLOR_CYAN        0x07FF
#define COLOR_MAGENTA     0xF81F
#define COLOR_YELLOW      0xFFE0
#define COLOR_WHITE       0xFFFF
#define COLOR_GRAY        0x8410
#define COLOR_ORANGE      0xFD20

void ST7735_Init(void);
void ST7735_FillScreen(uint16_t color);
void ST7735_DrawPixel(uint8_t x, uint8_t y, uint16_t color);
void ST7735_DrawChar(uint8_t x, uint8_t y, char c, uint16_t color);
void ST7735_DrawString(uint8_t x, uint8_t y, const char *str, uint16_t color);

#endif
